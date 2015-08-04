#include "opo.h"
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/gptimer.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "rf_switch.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "dev/rfcore-xreg.h"
#include "dev/cctest.h"
#include "simple_sfd_handler.h"
#include "rtc.h"
#include "dev/leds.h"
#include "vtimer.h"
#include "dev/cc2538-rf.h"
#include "cc2538-rf-debug.h"
#include "cpu.h"

/*
	Opo functions
	Current version uses comp1 or everything rx
*/

PROCESS(opo_rx, "OpoRx");
PROCESS(opo_tx, "OpoTx");
PROCESS(opo_restart, "OpoRestart");
PROCESS(opo_rx_noise, "OpoRxNoise");

static enum {OPO_RX, OPO_RX_SETUP, OPO_TX, OPO_IDLE} opo_state = OPO_IDLE; // Higher level opo state
static uint8_t opo_tx_stage = 0;

static uint32_t ul_wakeup_time = 0; // Ultrasonic pulse receive time
static uint32_t sfd_time = 0; // SFD receive time
static uint32_t tx_start_time = 0; // UL pulse transmit time, based on sleep timer
static bool rf_packet_received = false;
static bool transmission_noise = true; // switching from tx to rx causes a voltage shift, leading to a false wakeup
uint32_t failed_rx_count = 0;

static opo_rx_callback_t opo_rx_callback;
static void (*opo_tx_callback)();

volatile static opo_rmsg_t  txmsg;
static          opo_rmsg_t  rxmsg_storage;
volatile static opo_data_t  rxmsg;
static opo_meta_t  meta;

static struct vtimer rx_vt; // used during an opo reception
static struct vtimer rx_restart_vt;
static struct vtimer tx_vt;

static void enable_opo_ul_rx();
static void enable_opo_ul_tx();


/* ************************************************************************** */
// Opo RX shit
static void default_opo_rx_callback(opo_data_t odata) {}

// Callback for when we receive an ultrasonic wake up
static void wakeup_callback(uint8_t port, uint8_t pin) {
	GPIO_DISABLE_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	GPIO_DISABLE_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
	ul_wakeup_time = VTIMER_NOW();
	sfd_time = 0;
	//leds_on(LEDS_RED);
	if(opo_state == OPO_IDLE) {
		opo_state = OPO_RX;
		process_poll(&opo_rx);
	}
}

// Callback for SFD trigger. Time to aquire
static void rx_sfd_callback() {
	if(opo_state == OPO_RX && sfd_time == 0) { sfd_time = VTIMER_NOW(); }
}

static void rf_rx_callback() {
	if(opo_state == OPO_RX) {
		uint16_t packet_length = packetbuf_datalen();
		if(packet_length == sizeof(opo_rmsg_t)) {
			packetbuf_copyto((void *) &rxmsg_storage);
			packetbuf_clear();
			if(rxmsg_storage.preamble == (uint16_t) ~(rxmsg_storage.id)) { // need cast because C
				rf_packet_received = true;
				process_poll(&opo_rx);
			}
		}
	}

}

static void tx_rf_rx_done_callback() {}

static void rx_vt_callback() {
	if(opo_state == OPO_RX) {
		process_poll(&opo_rx);
	}
}

PROCESS_THREAD(opo_rx_noise, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		enable_opo_rx();
	}
	PROCESS_END();
}

PROCESS_THREAD(opo_rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(opo_state == OPO_RX) {
				if(!transmission_noise) {
					cc2538_ant_enable();
					packetbuf_clear();
					NETSTACK_MAC.on();
				}
				schedule_vtimer(&rx_vt, VTIMER_SECOND/1000 * 20); // fallback timer
			PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
				packetbuf_clear();
				NETSTACK_MAC.off(0);
				opo_state = OPO_IDLE;
				//leds_off(LEDS_RED);
				if(rf_packet_received == true) {
					uint32_t diff = (uint32_t) (sfd_time - ul_wakeup_time);
					if(diff < rxmsg_storage.ul_rf_dt) {
						rxmsg.tx_id = rxmsg_storage.id;
						rxmsg.tx_unixtime = rxmsg_storage.unixtime;
						rxmsg.tx_time_confidence = rxmsg_storage.time_confidence;
						rxmsg.m_unixtime = rtc_get_unixtime();
						rxmsg.m_time_confidence = meta.time_confidence;
						rxmsg.range_dt = (uint32_t)(rxmsg_storage.ul_rf_dt - diff);
						rxmsg.failed_rx_count = failed_rx_count;
						// Put extra info in transmit package
						txmsg.last_interaction_partner_id = rxmsg.tx_id;
						txmsg.last_unixtime = rxmsg.m_unixtime;
						txmsg.last_range_dt = rxmsg.range_dt;
						rf_packet_received = false;
						(opo_rx_callback)(rxmsg);
					}
					else {
						failed_rx_count++;
						rf_packet_received = false;
						enable_opo_rx();
					}
				}
				else {
					failed_rx_count++;
					enable_opo_rx();
				}
		}
	}
	PROCESS_END();
}

/* ************************************************************************** */
// Opo TX Callbacks and Processes
static void default_opo_tx_callback() {}

// Callback for the hardware interrupt indicating that the packet was sent
static void rf_txdone_callback() {
	if(opo_state == OPO_TX && opo_tx_stage == 1) {
		opo_tx_stage = 2;
		process_poll(&opo_tx);
	}
}

// Callback for SFD trigger.
static void tx_sfd_callback() {}

static void tx_vt_callback() {
	if(opo_tx_stage == 1) {
		process_poll(&opo_tx);
	}
}

PROCESS_THREAD(opo_tx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
			if(opo_tx_stage == 0) {
				txmsg.id = (uint16_t) meta.id;
				txmsg.preamble = (uint16_t) ~(txmsg.id);
				txmsg.time_confidence = meta.time_confidence;
				txmsg.unixtime = rtc_get_unixtime();
				tx_start_time = VTIMER_NOW();
				gpt_enable_event(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER);
				enable_opo_ul_tx();
				clock_delay_usec(700);
				gpt_disable_event(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER);
				opo_tx_stage = 1;
				schedule_vtimer(&tx_vt, (VTIMER_SECOND/1000 * 7));
			}
			else {
				goto opo_tx_cleanup;
			}
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(opo_tx_stage == 1) {
			cc2538_ant_enable();
			packetbuf_clear();
			packetbuf_copyfrom((void *) &txmsg, sizeof(opo_rmsg_t));
			cc2538_on_and_transmit(tx_start_time);
			PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
				if(opo_tx_stage == 2) {
					opo_tx_cleanup:
						NETSTACK_MAC.off(0);
						opo_tx_stage = 0;
						opo_state = OPO_IDLE;
						transmission_noise = true;
						(*opo_tx_callback)();
				} else {
					goto opo_tx_cleanup;
				}
		}
		else {
			goto opo_tx_cleanup;
		}
	}
	PROCESS_END();
}


/* ************************************************************************** */

static void restart_vt_callback() {
	process_poll(&opo_restart);
}


PROCESS_THREAD(opo_restart, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(opo_state == OPO_RX_SETUP) {
			opo_state = OPO_IDLE;
			GPIO_ENABLE_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
			GPIO_ENABLE_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
		}
	}
	PROCESS_END();
}

static void setup_opo_rx() {
	SFD_HANDLER.set_callback(rx_sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(rf_txdone_callback);
  	simple_network_set_callback(rf_rx_callback);
  	ul_wakeup_time = 0;
  	sfd_time = 0;
	rf_packet_received = false;
	GPIO_SOFTWARE_CONTROL(OPO_PWM_PORT_BASE, OPO_PWM_PIN_MASK);
	GPIO_SET_OUTPUT(OPO_PWM_PORT_BASE, OPO_PWM_PIN_MASK);
	GPIO_SET_PIN(OPO_PWM_PORT_BASE, OPO_PWM_PIN_MASK);
	enable_opo_ul_rx();
}

static void enable_opo_ul_rx() {
	GPIO_SET_OUTPUT(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
  	GPIO_SET_PIN(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
}

static void enable_opo_ul_tx() {
	GPIO_SET_OUTPUT(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
  	GPIO_CLR_PIN(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
}

uint8_t enable_opo_rx() {
	INTERRUPTS_DISABLE();
	if(opo_state == OPO_IDLE) {
		opo_state = OPO_RX_SETUP;
		setup_opo_rx();
		uint8_t dl = 20;
		if(transmission_noise) {
			transmission_noise = false;
			dl += 70;
		}
		schedule_vtimer(&rx_restart_vt, VTIMER_SECOND/1000 * dl);
		INTERRUPTS_ENABLE();
		return 1;
	}
	INTERRUPTS_ENABLE();
	return 0;
}

inline void disable_opo_rx() {
	GPIO_DISABLE_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	GPIO_DISABLE_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
}

void setup_40kh_pwm() {
	ungate_gpt(OPO_PWM_GPTIMER);
	gpt_set_16_bit_timer(OPO_PWM_GPTIMER);
	gpt_set_mode(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER, GPTIMER_PERIODIC_MODE);
	gpt_set_alternate_mode(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER, GPTIMER_ALTERNATE_MODE_PWM);
	gpt_set_interval_value(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER, 0x190);
	gpt_set_match_value(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER, 0xC8);
	ioc_set_sel(OPO_PWM_PORT_NUM, OPO_PWM_PIN_NUM, IOC_PXX_SEL_GPT1_ICP1);
  	ioc_set_over(OPO_PWM_PORT_NUM, OPO_PWM_PIN_NUM, IOC_OVERRIDE_OE);
  	GPIO_PERIPHERAL_CONTROL(OPO_PWM_PORT_BASE, OPO_PWM_PIN_MASK);
}


static void setup_opo_tx() {
	SFD_HANDLER.set_callback(tx_sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(rf_txdone_callback);
  	simple_network_set_callback(tx_rf_rx_done_callback);
  	opo_tx_stage = 0;
  	setup_40kh_pwm();
}

static void setup_opo_rx_pins() {
	GPIO_SET_INPUT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	ioc_set_over(OPO_COMP1_PORT_NUM, OPO_COMP1_PIN_NUM, IOC_OVERRIDE_DIS);
	GPIO_DETECT_EDGE(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	GPIO_DETECT_RISING(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
}

void opo_init() {
	meta.id = OPO_ID;
	meta.time_confidence = 0;
	rxmsg.version_num = 0xaabb;
	rxmsg.rx_id = OPO_ID;
	setup_40kh_pwm();
	setup_opo_rx_pins();
	nvic_interrupt_enable(OPO_RX_NVIC);
	register_opo_rx_callback(default_opo_rx_callback);
	register_opo_tx_callback(default_opo_tx_callback);
	gpio_register_callback((gpio_callback_t) wakeup_callback, OPO_RX_PORT_NUM, OPO_RX_PIN_NUM);
	process_start(&opo_rx, NULL);
	process_start(&opo_tx, NULL);
	process_start(&opo_restart, NULL);
	process_start(&opo_rx_noise, NULL);
	rx_vt = get_vtimer(rx_vt_callback);
	tx_vt = get_vtimer(tx_vt_callback);
	tx_vt.debug_counter = 2;
	rx_restart_vt = get_vtimer(restart_vt_callback);
	GPIO_POWER_UP_ON_RISING(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
}

void register_opo_rx_callback(opo_rx_callback_t mcallback) {
	opo_rx_callback = mcallback;
}

void register_opo_tx_callback(void *mcallback) {
	opo_tx_callback = mcallback;
}

uint8_t perform_opo_tx() {
	INTERRUPTS_DISABLE();
	if(opo_state == OPO_IDLE) {
		opo_state = OPO_TX;
		disable_opo_rx();
		setup_opo_tx();
		process_poll(&opo_tx);
		INTERRUPTS_ENABLE();
		return 1;
	}
	INTERRUPTS_ENABLE();
	return 0;
}

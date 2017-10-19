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

#define OPO_ID 4

PROCESS(opo_rx, "OpoRx");
PROCESS(opo_tx, "OpoTx");
PROCESS(opo_restart, "OpoRestart");

static enum {OPO_RX, OPO_RX_RF, OPO_TX, OPO_IDLE} opo_state = OPO_IDLE; // Higher level opo state
static uint8_t opo_tx_stage = 0;

static uint32_t ul_wakeup_time = 0; // Ultrasonic pulse receive time
static uint32_t sfd_time = 0; // SFD receive time
static uint32_t ul_count = 0; // number of ul pulses received
static uint32_t tx_start_time = 0; // UL pulse transmit time, based on sleep timer
static bool rf_packet_received = false;
static bool transmission_noise = false; // switching from tx to rx causes a voltage shift, leading to a false wakeup
static bool self_reset_rx = false; // if it was not a successful ranging operation, we self-reset opo rx
uint32_t failed_rx_count = 0;

static opo_rx_callback_t opo_rx_callback;
static void (*opo_tx_callback)();

volatile static opo_rmsg_t  txmsg;
volatile static opo_data_t  rxmsg;
static opo_rmsg_t           rxmsg_storage;
static opo_meta_t           meta;

static struct vtimer rx_vt; // used during an opo reception
static struct vtimer rx_ul_counter_vt;
static struct vtimer rx_restart_vt;
static struct vtimer tx_vt;
static struct vtimer tx_failsafe_vt;

static void enable_opo_ul_rx();
static void enable_opo_ul_tx();

uint8_t get_opo_state() {
	return opo_state;
}

/* ************************************************************************** */
// Opo RX shit
static void default_opo_rx_callback(opo_data_t odata) {}
static void opo_rx_rf_default_callback() {}
/*
 * Callback used to check how many ultrasonic pules we received. Based on our transmission time,
 * we expect to receive at least 40 pulses on our comparator pin. If we don't, we assume this is
 * just some noise from multipath
 */
static void opo_rx_ul_count_checker() {
	GPIO_DISABLE_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	GPIO_DISABLE_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
	if(ul_count < 40) {
		transmission_noise = true;
	}
	ul_count = 0;
	process_poll(&opo_rx);
}

// Callback for when we receive an ultrasonic wake up
static void wakeup_callback(uint8_t port, uint8_t pin) {
	//GPIO_DISABLE_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	//GPIO_DISABLE_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
	if(opo_state == OPO_IDLE) {
		ul_wakeup_time = VTIMER_NOW();
		sfd_time = 0;
		opo_state = OPO_RX;
		ul_count++;
		schedule_vtimer_ms(&rx_ul_counter_vt, 2);
		//process_poll(&opo_rx);
	}
	else if(opo_state == OPO_RX) {
		ul_count++;
	}
}

// Callback for SFD trigger. Time to aquire
static void rx_sfd_callback() {
	if(opo_state == OPO_RX && sfd_time == 0) { sfd_time = VTIMER_NOW(); }
}

/*
 * Callback for when Opo receives an rf packet as part of a ranging event.
 * This callback is NOT in an interrupt context, so we must call INTERRUPT_DISABLE for atomic operations.
 * We do a basic check to see if this packet is a valid Opo packet, and then further process it in the opo_rx process
 * along with the ultrasonic and SFD time of arrivals for a complete opo ranging event.
*/
static void rf_rx_callback() {
	INTERRUPTS_DISABLE();
	/* Check to make sure we are in expected Opo state */
	if(opo_state == OPO_RX) {
		/* If packet length and preamble check out, process packet for Opo ranging event */
		uint16_t packet_length = packetbuf_datalen();
		if(packet_length == sizeof(opo_rmsg_t)) {
			/* We have to copy data out of the packetbuf before checking the preamble */
			packetbuf_copyto((void *) &rxmsg_storage);
			packetbuf_clear();
			if(rxmsg_storage.preamble == (uint16_t) ~(rxmsg_storage.id)) { // need cast because C or shitty compiler
				NETSTACK_MAC.off(0);
				cancel_vtimer(&rx_vt);
				opo_state = OPO_RX_RF;
				rf_packet_received = true;
				process_poll(&opo_rx);
			}
		}
	}
	INTERRUPTS_ENABLE();
}

static void tx_rf_rx_done_callback() {}

static void rx_vt_callback() {
	if(opo_state == OPO_RX) {
		//send_rf_debug_msg("opo.c: opo rx vt failsafe");
		opo_state = OPO_RX_RF;
		NETSTACK_MAC.off(0);
		process_poll(&opo_rx);
	}
	else {
		//send_rf_debug_msg("opo.c: rx vt failsafe fail");
	}
}

PROCESS_THREAD(opo_rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(opo_state == OPO_RX) {
			if(!transmission_noise) {
				//send_rf_debug_msg("opo.c: opo rx first stage\n");
				cc2538_ant_enable();
				packetbuf_clear();
				NETSTACK_MAC.on();
			}
			schedule_vtimer_ms(&rx_vt, 20); // fallback timer
			PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
				packetbuf_clear();
				NETSTACK_MAC.off(0);
				if(transmission_noise) {
					// Ultrasonic noise, probably from an opo transmission
					transmission_noise = false;
					self_reset_rx = true;
				}
				else if(rf_packet_received == true) {
					uint32_t diff = (uint32_t) (sfd_time - ul_wakeup_time);
					char buffer[100];
					snprintf(buffer, 100, "OPO: RX DIFF %lu %lu", diff, rxmsg_storage.ul_rf_dt);
					send_rf_debug_msg(buffer);
					if(diff < rxmsg_storage.ul_rf_dt) {
						//send_rf_debug_msg("opo.c: Opo rx got rf packet");
						rxmsg.tx_id = rxmsg_storage.id;
						rxmsg.tx_unixtime = rxmsg_storage.unixtime;
						//rxmsg.tx_time_confidence = rxmsg_storage.time_confidence;
						rxmsg.m_unixtime = rtc_get_unixtime();
						//rxmsg.m_time_confidence = meta.time_confidence;
						rxmsg.range_dt = (uint32_t)(rxmsg_storage.ul_rf_dt - diff);
						rxmsg.failed_rx_count = failed_rx_count;
						// Put extra info in transmit package
						txmsg.last_interaction_partner_id = rxmsg.tx_id;
						txmsg.last_unixtime = rxmsg.m_unixtime;
						txmsg.last_range_dt = rxmsg.range_dt;
						txmsg.failed_rx_count = failed_rx_count;
						rf_packet_received = false;
						self_reset_rx = false;
					}
					else {
						//send_rf_debug_msg("opo.c: Opo rx got rf packet, but invalid time\n");
						failed_rx_count++;
						rf_packet_received = false;
						txmsg.failed_rx_count = failed_rx_count;
						self_reset_rx = true;
					}
				}
				else {
					//send_rf_debug_msg("opo.c: Opo rx did not get rf packet\n");
					failed_rx_count++;
					txmsg.failed_rx_count = failed_rx_count;
					self_reset_rx = true;
				}

				opo_state = OPO_IDLE;
				if(self_reset_rx) {
					// Ranging was incomplete. Re-enable opo rx
					self_reset_rx = false;
					enable_opo_rx();
				}
				else {
					// Ranging complete. Return control.
					SFD_HANDLER.set_callback(opo_rx_rf_default_callback);
				  	RF_TXDONE_HANDLER.set_callback(opo_rx_rf_default_callback);
				  	simple_network_set_callback(opo_rx_rf_default_callback);
					(opo_rx_callback)(rxmsg);
				}
		}
		/* Add a failsafe here to deal with opo bug / contiki process issue */
		else {
			//send_rf_debug_msg("opo_rx process failsafe");
			failed_rx_count++;
			rf_packet_received = false;
			opo_state = OPO_IDLE;
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
		NETSTACK_MAC.off(0);
		cancel_vtimer(&tx_failsafe_vt);
		opo_tx_stage = 2;
		process_poll(&opo_tx);
	}
	else {

	}
}

// Callback for SFD trigger.
static void tx_sfd_callback() {}

/* Timer callback that triggers Opo to transmit a ranging rf packet */
static void tx_vt_callback() {
	if(opo_state == OPO_TX && opo_tx_stage == 1) {
		process_poll(&opo_tx);
	}
}

/* Timer callback that's used in case Opo fails to transmit a ranging rf packet */
static void tx_failsafe_vt_callback() {
	if(opo_state == OPO_TX && opo_tx_stage == 1) {
		//send_rf_debug_msg("opo.c: tx failsafe vt\n");
		NETSTACK_MAC.off(0);
		opo_tx_stage = 2;
		process_poll(&opo_tx);
	}
	else {
		//send_rf_debug_msg("opo.c: tx_failsafe_vt fail?\n");
	}
}

PROCESS_THREAD(opo_tx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
			if(opo_state == OPO_TX && opo_tx_stage == 0) {
				//send_rf_debug_msg("Opo tx stage 0\n");
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
				schedule_vtimer_ms(&tx_vt, 7);
			}
			else {
				//send_rf_debug_msg("Opo tx stage 0 fail\n");
				goto opo_tx_cleanup;
			}
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
			if(opo_state == OPO_TX && opo_tx_stage == 1) {
				//send_rf_debug_msg("Opo tx stage 1\n");
				SFD_HANDLER.set_callback(tx_sfd_callback);
			  	RF_TXDONE_HANDLER.set_callback(rf_txdone_callback);
			  	simple_network_set_callback(tx_rf_rx_done_callback);
				cc2538_ant_enable();
				packetbuf_clear();
				packetbuf_copyfrom((void *) &txmsg, sizeof(opo_rmsg_t));
				schedule_vtimer_ms(&tx_failsafe_vt, 20);
				cc2538_on_and_transmit(tx_start_time);
				PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
					if(opo_state == OPO_TX && opo_tx_stage == 2) {
						//send_rf_debug_msg("Opo tx stage 2\n");
						opo_tx_cleanup:
							NETSTACK_MAC.off(0);
							opo_tx_stage = 0;
							opo_state = OPO_IDLE;
							SFD_HANDLER.set_callback(default_opo_tx_callback);
						  	RF_TXDONE_HANDLER.set_callback(default_opo_tx_callback);
						  	simple_network_set_callback(default_opo_tx_callback);
							//transmission_noise = true;
							(*opo_tx_callback)();
					} else {
						//send_rf_debug_msg("Opo tx stage 2 fail\n");
						goto opo_tx_cleanup;
					}
			}
			else {
				//send_rf_debug_msg("Opo tx stage 1 fail\n");
				goto opo_tx_cleanup;
			}
	}
	PROCESS_END();
}


/* ************************************************************************** */

static void restart_vt_callback() {
	//send_rf_debug_msg("opo.c: restart_vt_callback\n");
	process_poll(&opo_restart);
}


PROCESS_THREAD(opo_restart, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		INTERRUPTS_DISABLE();
		if(opo_state == OPO_IDLE) {
			//send_rf_debug_msg("opo.c: Opo rx restart process success\n");
			GPIO_CLEAR_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
			GPIO_CLEAR_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
			GPIO_ENABLE_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
			GPIO_ENABLE_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
			GPIO_CLEAR_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
			GPIO_CLEAR_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
		}
		else {
			//send_rf_debug_msg("opo.c: Opo rx restart process fail\n");
		}
		INTERRUPTS_ENABLE();
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

bool enable_opo_rx() {
	INTERRUPTS_DISABLE();
	bool status = false;
	if(opo_state == OPO_IDLE) {
		setup_opo_rx();
		uint32_t dl = 40;

		schedule_vtimer_ms(&rx_restart_vt, dl);
		status = true;
	}
	if(!status) {
		if(opo_state == OPO_RX) {
			//send_rf_debug_msg("opo.c: enable_opo_rx fail OPO_RX\n");
		}
		else if(opo_state == OPO_RX_RF) {
			//send_rf_debug_msg("opo.c: enable_opo_rx fail OPO_RX_RF\n");
		}
		else if(opo_state == OPO_TX) {
			//send_rf_debug_msg("opo.c: enable_opo_rx fail OPO_TX\n");
		}
		else if(opo_state == OPO_IDLE) {
			//send_rf_debug_msg("opo.c: enable_opo_rx fail OPO_IDLE\n");
		}
		else {
			//send_rf_debug_msg("opo.c: enable_opo_rx fail wtf\n");
		}
	}

	INTERRUPTS_ENABLE();
	return status;
}

inline void disable_opo_rx() {
	GPIO_DISABLE_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	GPIO_DISABLE_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(OPO_RX_PORT_BASE, OPO_RX_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
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
	GPIO_POWER_UP_ON_RISING(OPO_RX_PORT_NUM, OPO_RX_PIN_MASK);
}

void opo_init() {
	meta.id = OPO_ID;
	meta.time_confidence = 0;
	rxmsg.version_num = 0x01;
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
	rx_vt = get_vtimer(rx_vt_callback);
	rx_ul_counter_vt = get_vtimer(opo_rx_ul_count_checker);
	rx_restart_vt = get_vtimer(restart_vt_callback);
	tx_vt = get_vtimer(tx_vt_callback);
	tx_failsafe_vt = get_vtimer(tx_failsafe_vt_callback);
	opo_state = OPO_IDLE;
}

void register_opo_rx_callback(opo_rx_callback_t mcallback) {
	opo_rx_callback = mcallback;
}

void register_opo_tx_callback(void *mcallback) {
	opo_tx_callback = mcallback;
}

bool perform_opo_tx() {
	INTERRUPTS_DISABLE();
	bool status = false;
	if(opo_state == OPO_IDLE) {
		//send_rf_debug_msg("opo.c: Perform opo tx success\n");
		opo_state = OPO_TX;
		opo_tx_stage = 0;
		disable_opo_rx();
		setup_opo_tx();
		process_poll(&opo_tx);
		status = true;
	}
	if(!status) {
		if(opo_state == OPO_RX) {
			send_rf_debug_msg("opo.c: perform_opo_tx fail OPO_RX\n");
		}
		else if(opo_state == OPO_RX_RF) {
			send_rf_debug_msg("opo.c: perform_opo_tx fail OPO_RX_RF\n");
		}
		else if(opo_state == OPO_TX) {
			send_rf_debug_msg("opo.c: perform_opo_tx fail OPO_TX\n");
		}
		else if(opo_state == OPO_IDLE) {
			send_rf_debug_msg("opo.c: perform_opo_tx fail OPO_IDLE\n");
		}
		else {
			send_rf_debug_msg("opo.c: perform_opo_tx fail wtf\n");
		}
	}
	INTERRUPTS_ENABLE();
	return status;
}

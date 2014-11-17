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

/*
	Opo functions
	Current version uses comp1 or everything rx
*/

PROCESS(opo_rx, "OpoRx");
PROCESS(opo_tx, "OpoTx");

enum {OPO_RX, OPO_TX, OPO_IDLE} opo_state = OPO_IDLE; // Higher level opo state
enum {OPO_RX_IDLE, OPO_RX_WOKEN, OPO_RX_RANGING} opo_rx_state = OPO_RX_IDLE; // Current state in RX
uint8_t opo_tx_stage = 0;
uint32_t ul_rf_dt = 0; // RF/UL TDoA for ranging
uint32_t ul_dt = 0; //UL TD for angle. Currently unused
bool ul_received = false;
bool rf_received = false;
bool rx_success = false;


bool opo_rx_callback_set = false;
opo_rx_callback_t opo_rx_callback;

bool opo_tx_callback_set = false;
void (*opo_tx_callback)();

opo_rxmsg_t rxmsg;
opo_rmsg_t  *txmsg;

static struct etimer rx_et;
static struct etimer tx_et;

/* ************************************************************************** */
// Opo RX shit

// Callback for when we receive an ultrasonic wake us
static void wakeup_callback(uint8_t port, uint8_t pin) {
	GPIO_DISABLE_INTERRUPT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	if(opo_state == OPO_IDLE && opo_rx_state == OPO_RX_IDLE) {
		opo_state = OPO_RX;
		opo_rx_state = OPO_RX_WOKEN;
		GPIO_PERIPHERAL_CONTROL(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
		process_poll(&opo_rx);
	}
}

// Callback for SFD trigger. Time to aquire
static void rx_sfd_callback() {
	if(opo_state == OPO_RX && opo_rx_state == OPO_RX_WOKEN) {
		opo_rx_state = OPO_RX_RANGING;
		REG(OPO_COMP1_GPT_BASE | OPO_COMP1_GPT_TV) = 0;
		gpt_enable_event(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER);
	}
}

static void rf_rx_callback() {
	if(opo_state == OPO_RX) {
		uint16_t packet_length = packetbuf_datalen();
		if(packet_length == sizeof(opo_rmsg_t)) {
			opo_rmsg_t *m = packetbuf_dataptr();
			if(m->preamble == OPO_PREAMBLE) {
				uint8_t i = 0;
				rf_received = true;
				rxmsg.tx_id = m->tx_id;
				rxmsg.time_confidence = m->time_confidence;
				for(i=0; i < 8; i ++)
					rxmsg.tx_full_time[i] = m->tx_full_time[i];
	    		rxmsg.tx_reset_counter = m->tx_reset_counter;
			}
		}

		if(ul_received == true) {
			etimer_stop(&rx_et);
			process_poll(&opo_rx);
		}
	}

}

// Allows us to keep time across timer overflows
static void overflow_callback(uint8_t timer,
							  uint8_t subtimer,
							  uint8_t function,
							  uint32_t gpt_time) {
	ul_rf_dt += 65535;
	gpt_clear_interrupt(OPO_COMP1_GPTIMER, OPO_COMP1_OFINT);
}

// callback for our UL ranging pulse
static void ul_cap_callback(uint8_t timer,
				            uint8_t subtimer,
				            uint8_t function,
				            uint32_t gpt_time) {
	ul_received = true;
	ul_rf_dt += gpt_time;

	gpt_disable_event(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER);
	gpt_clear_interrupt(OPO_COMP1_GPTIMER, OPO_COMP1_CAPINT);

	if(rf_received == true) {
		etimer_stop(&rx_et);
		process_poll(&opo_rx);
	}
}



PROCESS_THREAD(opo_rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
			etimer_set(&rx_et, CLOCK_SECOND/1000 * 45); // Sleep until we can really expect the radio packet
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
			cc2538_ant_enable();
			NETSTACK_MAC.on(); // turn on ze radio
			etimer_set(&rx_et, CLOCK_SECOND); // failsafe timer
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER || ev == PROCESS_EVENT_POLL);
			packetbuf_clear();
			NETSTACK_MAC.off(0);
			if(ul_received == true && rf_received == true) {
				rx_success = true;
			} else {
				rx_success = false;
				gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
				gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_CBECINT);
			}
			opo_state = OPO_IDLE;
			opo_rx_state = OPO_RX_IDLE;
			opo_rx_callback(rx_success, ul_rf_dt, ul_dt, rxmsg);
	}

	PROCESS_END();
}

/* ************************************************************************** */
// Opo TX Callbacks and Processes
static void rf_txdone_callback() {
	if(opo_state == OPO_TX) {
		opo_tx_stage = 3;
	}
}

// Callback for SFD trigger. 
static void tx_sfd_callback() {
	if(opo_state == OPO_TX) {
		opo_rx_state = OPO_RX_RANGING;
		gpt_enable_event(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER);
	}
}

PROCESS_THREAD(opo_tx, ev, data) {
	PROCESS_BEGIN();

	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
			if(opo_tx_stage == 0) {
				gpt_enable_event(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER);
				opo_tx_stage = 1;
				etimer_set(&tx_et, CLOCK_SECOND/1000);
			}
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
			if(opo_tx_stage == 1) {
				gpt_disable_event(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER);
				opo_tx_stage = 2;
				etimer_set(&tx_et, CLOCK_SECOND/1000 * 70);
			}
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
			if (opo_tx_stage == 2) {
				packetbuf_clear();
				packetbuf_copyfrom((void *) txmsg, sizeof(opo_rmsg_t));
				cc2538_ant_enable();
				NETSTACK_MAC.on();
				NETSTACK_MAC.send(NULL, NULL);
				
				PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
					NETSTACK_MAC.off(0);
					etimer_set(&tx_et, CLOCK_SECOND/1000);
			}
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
			if (opo_tx_stage == 3) {
				NETSTACK_MAC.off(0);
				gpt_disable_event(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER);
				opo_tx_stage = 0;
				opo_state = OPO_IDLE;
				(*opo_tx_callback)();
			}

	}
	PROCESS_END();
}


/* ************************************************************************** */

void enable_opo_ul_rx() {
	GPIO_SET_OUTPUT(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
  	GPIO_CLR_PIN(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
}

void enable_opo_ul_tx() {
	GPIO_SET_OUTPUT(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
  	GPIO_SET_PIN(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
}

void setup_40kh_pwm() {
	gpt_set_mode(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER, GPTIMER_PERIODIC_MODE);
	gpt_set_alternate_mode(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER, GPTIMER_ALTERNATE_MODE_PWM);
	gpt_set_interval_value(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER, 0x190);
	gpt_set_match_value(OPO_PWM_GPTIMER, OPO_PWM_GPSUBTIMER, 0xC8);
	ioc_set_sel(OPO_PWM_PORT_NUM, OPO_PWM_PIN_NUM, IOC_PXX_SEL_GPT1_ICP1);
  	ioc_set_over(OPO_PWM_PORT_NUM, OPO_PWM_PIN_NUM, IOC_OVERRIDE_OE);
  	GPIO_PERIPHERAL_CONTROL(OPO_PWM_PORT_BASE, OPO_PWM_PIN_MASK);
}

void setup_comp1_timer() {
	gpt_set_mode(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_CAPTURE_MODE);
	gpt_set_capture_mode(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_CAPTURE_MODE_EDGE_TIME);
	gpt_set_alternate_mode(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_ALTERNATE_MODE_CAPTURE);
	gpt_set_count_dir(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_COUNT_DIR_UP);
	gpt_set_event_mode(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_EVENT_MODE_POSITIVE_EDGE);
	gpt_enable_interrupt(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_CAPTURE_EVENT_INT);
	gpt_enable_interrupt(OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_TIMEOUT_EVENT);
	REG(OPO_COMP1_GPT_OCP) = OPO_GPT_COMP1_PIN;
}

void setup_comp2_timer() {
	gpt_set_mode(OPO_COMP2_GPTIMER, OPO_COMP2_GPSUBTIMER, GPTIMER_CAPTURE_MODE);
	gpt_set_capture_mode(OPO_COMP2_GPTIMER, OPO_COMP2_GPSUBTIMER, GPTIMER_CAPTURE_MODE_EDGE_TIME);
	gpt_set_alternate_mode(OPO_COMP2_GPTIMER, OPO_COMP2_GPSUBTIMER, GPTIMER_ALTERNATE_MODE_CAPTURE);
	gpt_set_count_dir(OPO_COMP2_GPTIMER, OPO_COMP2_GPSUBTIMER, GPTIMER_COUNT_DIR_UP);
	gpt_set_event_mode(OPO_COMP2_GPTIMER, OPO_COMP2_GPSUBTIMER, GPTIMER_EVENT_MODE_POSITIVE_EDGE);
	gpt_enable_interrupt(OPO_COMP2_GPTIMER, OPO_COMP2_GPSUBTIMER, GPTIMER_CAPTURE_EVENT_INT);
	gpt_enable_interrupt(OPO_COMP2_GPTIMER, OPO_COMP2_GPSUBTIMER, GPTIMER_TIMEOUT_EVENT);
	REG(OPO_COMP2_GPT_OCP) = OPO_GPT_COMP2_PIN;
}

void setup_integrator_timer() {
	gpt_set_mode(OPO_INT_GPTIMER, OPO_INT_GPSUBTIMER, GPTIMER_CAPTURE_MODE);
	gpt_set_capture_mode(OPO_INT_GPTIMER, OPO_INT_GPSUBTIMER, GPTIMER_CAPTURE_MODE_EDGE_TIME);
	gpt_set_alternate_mode(OPO_INT_GPTIMER, OPO_INT_GPSUBTIMER, GPTIMER_ALTERNATE_MODE_CAPTURE);
	gpt_set_count_dir(OPO_INT_GPTIMER, OPO_INT_GPSUBTIMER, GPTIMER_COUNT_DIR_UP);
	gpt_set_event_mode(OPO_INT_GPTIMER, OPO_INT_GPSUBTIMER, GPTIMER_EVENT_MODE_POSITIVE_EDGE);
	gpt_enable_interrupt(OPO_INT_GPTIMER, OPO_INT_GPSUBTIMER, GPTIMER_CAPTURE_EVENT_INT);
	gpt_enable_interrupt(OPO_INT_GPTIMER, OPO_INT_GPSUBTIMER, GPTIMER_TIMEOUT_EVENT);
	REG(OPO_INT_GPT_OCP) = OPO_GPT_INT_PIN;
}


static inline void setup_opo_rx() {
	SFD_HANDLER.set_callback(rx_sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(rf_txdone_callback);
  	simple_network_set_callback(rf_rx_callback);
  	ul_rf_dt = 0;
	ul_dt = 0;
	ul_received = false;
	rf_received = false;
	rx_success = false;
	enable_opo_ul_rx();
}

static inline void setup_opo_tx() {
	SFD_HANDLER.set_callback(tx_sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(rf_txdone_callback);
  	simple_network_set_callback(rf_rx_callback);
  	opo_tx_stage = 0;
  	enable_opo_ul_tx();
}

void opo_init() {
	setup_40kh_pwm(); // setup 40 khz pwm
	setup_comp1_timer();
	setup_comp2_timer();
	setup_integrator_timer();
	gpt_register_callback(ul_cap_callback, OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_CAPTURE_EVENT);
	gpt_register_callback(overflow_callback, OPO_COMP1_GPTIMER, OPO_COMP1_GPSUBTIMER, GPTIMER_TIMEOUT_EVENT);
	gpio_register_callback(wakeup_callback, OPO_COMP1_PORT_NUM, OPO_COMP1_PIN_NUM);
	process_start(&opo_rx, NULL);
	process_start(&opo_tx, NULL);
}

uint8_t enable_opo_rx() {
	if(opo_state == OPO_IDLE) {
		setup_opo_rx();
		GPIO_SOFTWARE_CONTROL(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
		GPIO_ENABLE_INTERRUPT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
		return 1;
	}
	return 0;
}

void disable_opo_rx() {
	GPIO_SOFTWARE_CONTROL(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	GPIO_DISABLE_INTERRUPT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
}

void register_opo_rx_callback(opo_rx_callback_t mcallback) {
	opo_rx_callback = mcallback;
	opo_rx_callback_set = true;
}

void register_opo_tx_callback(void *mcallback) {
	opo_tx_callback = mcallback;
	opo_tx_callback_set = true;
}

uint8_t perform_opo_tx(opo_rmsg_t *msg) {
	if(opo_state == OPO_IDLE) {
		opo_state = OPO_TX;
		disable_opo_rx();
		txmsg = msg;
		setup_opo_tx();
		process_poll(&opo_tx);
		return 1;
	} 
	return 0;
}
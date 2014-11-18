#include "opo4rx.h"
#include "nrf8001.h"
#include "contiki.h"
#include "nrf8001.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/gptimer.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "rf_switch.h"
#include <stdio.h>
#include <stdbool.h>
#include "dev/rfcore-xreg.h"
#include "dev/cctest.h"
#include "simple_sfd_handler.h"
#include "dev/leds.h"
#include "opo_helper.h"

/*---------------------------------------------------------------------------*/
PROCESS(startup, "Opo4RxStartup");
PROCESS(opo_rx, "Opo4Rx");
AUTOSTART_PROCESSES(&startup);
/*---------------------s------------------------------------------------------*/

uint32_t test_data[6] = {66,66,66,66,66,66};
static struct etimer et;
uint32_t td = 0;
uint32_t initial_td = 0;
uint8_t started = 0;
uint32_t txcalled = 0;
uint32_t tx_sfd_done = 0;
uint8_t sfd_received = 0;
uint8_t test_stage = 0;
bool packet_received = false;
bool ul_received = false;

void sfd_callback() {
	if(sfd_received == 0 && started == 1) {
		sfd_received = 1;
		REG(GPTIMER_1_BASE | GPTIMER_TBV) = 0;
		gpt_enable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
	}
}

void tx_sfd_callback() {
	tx_sfd_done = 1;
}

void wake_up(uint8_t port, uint8_t pin) {
	GPIO_DISABLE_INTERRUPT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	if(started == 0) {
		started = 1;
		GPIO_PERIPHERAL_CONTROL(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
		process_poll(&opo_rx);
	}
}

void txdone_callback() {
	txcalled = 1;
	process_poll(&opo_rx);
}

void overflow_compensate(uint8_t timer, uint8_t subtimer, uint8_t function, uint32_t gpt_time) {
	td += 65535;
	gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_TBTOCINT);
}

void ul_cap(uint8_t timer, uint8_t subtimer, uint8_t function, uint32_t gpt_time) {
	gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
	gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_CBECINT);
	td += gpt_time;
	ul_received = true;
}

void rf_rx_callback() {
	packet_received = true;
	packetbuf_clear();
}


PROCESS_THREAD(opo_rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		etimer_set(&et, CLOCK_SECOND/1000 * 45);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		NETSTACK_MAC.on();
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);

		if(ul_received == true && packet_received == true) {
			leds_toggle(LEDS_BLUE);
			test_data[5] = td;
			test_data[4] = REG(GPTIMER_1_BASE | GPTIMER_TBPV);
			packetbuf_clear();
			packetbuf_copyfrom((void *) test_data, sizeof(test_data));
			NETSTACK_MAC.send(NULL, NULL);
			PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		} else {
			gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
			gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_CBECINT);
			NETSTACK_MAC.off(0);
		}

		etimer_set(&et, CLOCK_SECOND);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		td = 0;
		started = 0;
		sfd_received = 0;
		ul_received = false;
		packet_received = false;
		SFD_HANDLER.set_callback(sfd_callback);

		GPIO_SOFTWARE_CONTROL(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
		GPIO_ENABLE_INTERRUPT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
	}

	PROCESS_END();
}

PROCESS_THREAD(startup, ev, data) {
	PROCESS_BEGIN();

	// Setting all the radio shit up.
	packetbuf_clear();
	cc2538_ant_enable();
	simple_network_set_callback(rf_rx_callback);
	SFD_HANDLER.set_callback(sfd_callback);
	RF_TXDONE_HANDLER.set_callback(txdone_callback);

	// setup timers and pins
	opo_init();

	// Register callbacks for timer capture and timer overflow compensation.
	gpt_register_callback(ul_cap, GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_EVENT);
	gpt_register_callback(overflow_compensate, GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_TIMEOUT_EVENT);

  	// Setup wakeup pin callback.
  	gpio_register_callback(wake_up, OPO_COMP1_PORT_NUM, OPO_COMP1_PIN_NUM);

  	process_start(&opo_rx, NULL);

    GPIO_SOFTWARE_CONTROL(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
    GPIO_ENABLE_INTERRUPT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK); // enable wakeup pin interrupts


	PROCESS_END();
}

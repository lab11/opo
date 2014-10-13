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

/*---------------------------------------------------------------------------*/
PROCESS(startup, "Opo4RxStartup");
PROCESS(opo_rx, "Opo4Rx");
AUTOSTART_PROCESSES(&startup);
/*---------------------s------------------------------------------------------*/

uint32_t test_data[6] = {72,72,72,72,72,72};
static struct etimer et;
uint32_t td = 0;
uint32_t initial_td = 0;
uint8_t started = 0;
uint32_t txcalled = 0;
uint8_t sfd_received = 0;
uint8_t test_stage = 0;
bool packet_received = false;
bool ul_received = false;

void sfd_callback() {
	if(sfd_received == 0 && started == 1) {
		REG(GPTIMER_1_BASE | GPTIMER_TBV) = 0;
		gpt_enable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
		sfd_received = 1;
	}

}

void tx_sfd_callback() {
	leds_toggle(LEDS_RED);
}

void wake_up(uint8_t port, uint8_t pin) {
	GPIO_DISABLE_INTERRUPT(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
	if(started == 0) {
		started = 1;
		GPIO_PERIPHERAL_CONTROL(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
		process_poll(&opo_rx);
	}
}

void txdone_callback() {
	txcalled = 1;
}

void overflow_compensate(uint8_t timer, uint8_t subtimer, uint8_t function, uint32_t gpt_time) {
	td += 65535;
	gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_TBTOCINT);
}

void ul_cap(uint8_t timer, uint8_t subtimer, uint8_t function, uint32_t gpt_time) {
	gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
	td += gpt_time & 0xffff;
	ul_received = true;
	gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_CBECINT);
}

void rf_rx_callback() {
	packet_received = true;
	NETSTACK_MAC.off(0);
	packetbuf_clear();
}



PROCESS_THREAD(opo_rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		etimer_set(&et, CLOCK_SECOND/1000 * 60);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		NETSTACK_MAC.on();
		etimer_set(&et, 30);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		NETSTACK_MAC.off(0);
		if(ul_received == true && packet_received == true) {
			leds_on(LEDS_RED);
			leds_off(LEDS_GREEN);
			NETSTACK_MAC.on();
			packetbuf_clear();
			packetbuf_copyfrom((void *) test_data, 6);
			test_data[5] = td;
			SFD_HANDLER.set_callback(tx_sfd_callback);
			NETSTACK_MAC.send(NULL, NULL);
		} else {
			leds_on(LEDS_GREEN);
			leds_off(LEDS_RED);
		}
		td = 0;
		ul_received = false;
		packet_received = false;
		etimer_set(&et, 70);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		GPIO_SOFTWARE_CONTROL(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
		GPIO_ENABLE_INTERRUPT(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
	}

	PROCESS_END();
}

PROCESS_THREAD(startup, ev, data) {

	// Integrator Pin: PC7
	// COMP1: PC6
	// COMP2: PC5
	PROCESS_BEGIN();

	// Setting all the radio shit up.
	packetbuf_clear();
	cc2538_ant_enable();
	simple_network_set_callback(rf_rx_callback);
	SFD_HANDLER.set_callback(sfd_callback);
	RF_TXDONE_HANDLER.set_callback(txdone_callback);

	//Setting all the timer shit up
	ungate_gpt(GPTIMER_1);
	gpt_set_16_bit_timer(GPTIMER_1);

	gpt_set_mode(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_MODE);
	gpt_set_capture_mode(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_MODE_EDGE_TIME);
	gpt_set_alternate_mode(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_ALTERNATE_MODE_CAPTURE);
	gpt_set_count_dir(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_COUNT_DIR_UP);
	gpt_set_event_mode(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_EVENT_MODE_POSITIVE_EDGE);
	gpt_enable_interrupt(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_EVENT_INT);
	gpt_enable_interrupt(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_TIMEOUT_EVENT);
	REG(IOC_GPT1OCP2) = OPO_GPT_INT_PIN; // Integrator, PC7, pin 23

	// Register callbacks for timer capture and timer overflow compensation.
	gpt_register_callback(ul_cap, GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_EVENT);
	gpt_register_callback(overflow_compensate, GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_TIMEOUT_EVENT);

  	// Set up Opo Tx/Rx Control to RX
  	GPIO_SET_OUTPUT(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
  	GPIO_CLR_PIN(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);

  	// Setup wakeup (integrator) pin.
  	gpio_register_callback(wake_up, OPO_INT_PORT_NUM, OPO_INT_PIN_NUM);
	GPIO_SET_INPUT(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
	GPIO_DETECT_EDGE(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
	GPIO_DETECT_RISING(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK);
	ioc_set_over(OPO_INT_PORT_NUM, OPO_INT_PIN_NUM, IOC_OVERRIDE_DIS); // Disable pullup / pulldown functionality on integrator pin

  	//Setup COMP1 and COMP2 pins
  	GPIO_SET_INPUT(OPO_COMP1_PORT_BASE, OPO_COMP1_PIN_MASK);
  	GPIO_SET_INPUT(OPO_COMP2_PORT_BASE, OPO_COMP2_PIN_MASK);
  	ioc_set_over(OPO_COMP1_PORT_NUM, OPO_COMP1_PIN_NUM, IOC_OVERRIDE_DIS); // disable pullup/pulldown
  	ioc_set_over(OPO_COMP2_PORT_NUM, OPO_COMP2_PIN_NUM, IOC_OVERRIDE_DIS); // disable pullup/pulldown

  	process_start(&opo_rx, NULL);

   	nvic_interrupt_enable(OPO_INT_NVIC);
    nvic_interrupt_enable(NVIC_INT_GPTIMER_1B);

    GPIO_ENABLE_INTERRUPT(OPO_INT_PORT_BASE, OPO_INT_PIN_MASK); // enable wakeup pin interrupts

	PROCESS_END();
}

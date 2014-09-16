#include "opo3rx.h"
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

/*---------------------------------------------------------------------------*/
PROCESS(rx_test, "Opo3Rx");
PROCESS(rx_fallback, "Opo3RxFallback");
AUTOSTART_PROCESSES(&rx_test);
/*---------------------s------------------------------------------------------*/

uint32_t test_data[6] = {65,65,65,65,65,65};
static struct etimer et;
uint32_t td = 0;
uint8_t started = 0;
uint32_t called = 0;
uint32_t txcalled = 0;
uint8_t sfd_received = 0;
uint8_t test_stage = 0;

void sfd_callback() {
	if(sfd_received == 0 && started == 1) {
		sfd_received = 1;
	}

}

void wake_up(uint8_t port, uint8_t pin) {
	GPIO_DISABLE_INTERRUPT(GPIO_C_BASE, 0x80);
	GPIO_CLEAR_INTERRUPT(GPIO_C_BASE, 0x80);
	if(started == 0) {
		GPIO_CLR_PIN(GPIO_C_BASE, 0x02);
		started = 1;
		process_poll(&rx_fallback);
		gpt_enable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
		GPIO_PERIPHERAL_CONTROL(GPIO_C_BASE, 0x80);
		NETSTACK_MAC.on();
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
	GPIO_SET_PIN(GPIO_C_BASE, 0x80);
	gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
	td += gpt_time;
	gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_CBECINT);
}

void rf_rx_callback() {
	called = 1;
}

PROCESS_THREAD(rx_fallback, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		etimer_set(&et, 200);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);

		NETSTACK_MAC.off(0);
		test_stage = 1;
		etimer_set(&et, 150);

		if(sfd_received > 0) {
			test_data[3] = 60;
		}
		else {
			test_data[3] = 65;
		}

		if(td != 0) {
			test_data[4] = td;
		}
		else {
			test_data[4] = 65;
		}

		test_data[2] = REG(GPTIMER_1_BASE | GPTIMER_IMR);
		NETSTACK_MAC.on();
		packetbuf_clear();
		packetbuf_copyfrom((void *) test_data, 24);
		NETSTACK_MAC.send(NULL, NULL);
		NETSTACK_MAC.off(0);
		td = 0;
		started = 0;
		sfd_received = 0;
		test_stage = 0;

		GPIO_SOFTWARE_CONTROL(GPIO_C_BASE, 0x80);
		GPIO_CLEAR_INTERRUPT(GPIO_C_BASE, 0x80);
		GPIO_ENABLE_INTERRUPT(GPIO_C_BASE, 0x80); // time to listen for wake ups again

	}

	PROCESS_END();
}

PROCESS_THREAD(rx_test, ev, data) {

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
	//gpt_enable_interrupt(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_TIMEOUT_EVENT);
	REG(IOC_GPT1OCP2) = OPO_GPT_CAP_PIN; // Integrator, PC7, pin 23

	// Register callbacks for timer capture and timer overflow compensation.
	gpt_register_callback(ul_cap, GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_EVENT);
	gpt_register_callback(overflow_compensate, GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_TIMEOUT_EVENT);

	GPIO_SOFTWARE_CONTROL(GPIO_C_BASE, 0xFF);

  	// Set up Opo Tx/Rx Control to RX
  	GPIO_SET_OUTPUT(GPIO_C_BASE, 0x02);
  	GPIO_SET_PIN(GPIO_C_BASE, 0x02);

  	// Setup and enable wakeup (integrator) pin.
  	gpio_register_callback(wake_up, GPIO_C_NUM, 7);
	GPIO_SET_INPUT(GPIO_C_BASE, 0x80);
	GPIO_DETECT_EDGE(GPIO_C_BASE, 0x80);
	GPIO_TRIGGER_SINGLE_EDGE(GPIO_C_BASE, 0x80);
	GPIO_DETECT_RISING(GPIO_C_BASE, 0x80);
	GPIO_ENABLE_INTERRUPT(GPIO_C_BASE, 0x80);
	ioc_set_over(GPIO_C_NUM, 7, IOC_OVERRIDE_PDE);

  	//Setup COMP1 and COMP2 pins
  	GPIO_SET_INPUT(GPIO_C_BASE, 0x40);
  	GPIO_SET_OUTPUT(GPIO_C_BASE, 0x20);
  	GPIO_SET_PIN(GPIO_C_BASE, 0x20);

  	ioc_set_over(GPIO_C_NUM, 6, IOC_OVERRIDE_DIS);
  	ioc_set_over(GPIO_C_NUM, 5, IOC_OVERRIDE_DIS);

  	process_start(&rx_fallback, NULL);

   	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_C);
    nvic_interrupt_enable(NVIC_INT_GPTIMER_1B);

	PROCESS_END();
}

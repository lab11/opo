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
PROCESS(tx_test, "Opo3Tx");
AUTOSTART_PROCESSES(&tx_test);
/*---------------------s------------------------------------------------------*/

uint8_t test_data[6] = {65,65,65,65,65,65};
static struct etimer et;
uint32_t td = 0;
uint8_t started = 0;
uint32_t called = 0;

void sfd_callback() {
	REG(IOC_GPT1OCP2) = OPO_GPT_CAP_PIN; // Integrator, PC7, pin 24
	gpt_enable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
	nvic_interrupt_enable(NVIC_INT_GPTIMER_1A);
}

void wake_up(uint8_t port, uint8_t pin) {
	if(started == 0) {
		nvic_interrupt_disable(NVIC_INT_GPIO_PORT_C);
		NETSTACK_MAC.on();
		etimer_set(&et, 120);
	}
}

void overflow_compensate(uint8_t timer, uint8_t subtimer, uint8_t function, uint32_t gpt_time) {
	td += 65535;
	gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_TATOCINT);
}

void ul_cap(uint8_t timer, uint8_t subtimer, uint8_t function, uint32_t gpt_time) {
	gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_B);
	REG(IOC_GPT1OCP2) = 0;
	nvic_interrupt_disable(NVIC_INT_GPTIMER_1A);
	td += gpt_time;
	gpt_clear_interrupt(GPTIMER_1, GPTIMER_ICR_CAECINT);
}

void rf_rx_callback() {
	called += 1;
}

PROCESS_THREAD(tx_test, ev, data) {

	// Integrator Pin: PC7
	// COMP1: PC6
	// COMP2: PC5
	PROCESS_BEGIN();

	// Setting all the radio shit up.
	packetbuf_clear();
	cc2538_ant_enable();
	simple_network_set_callback(rf_rx_callback);
	SFD_HANDLER.set_callback(sfd_callback);

	//Setting all the timer shit up
	ungate_gpt(GPTIMER_1);
	gpt_set_16_bit_timer(GPTIMER_1);

	gpt_set_mode(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_MODE);
	gpt_set_capture_mode(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_MODE_EDGE_TIME);
	gpt_set_alternate_mode(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_ALTERNATE_MODE_CAPTURE);
	gpt_set_count_dir(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_COUNT_DIR_UP);
	gpt_set_event_mode(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_EVENT_MODE_POSITIVE_EDGE);
	gpt_enable_interrupt(GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_EVENT_INT);

	// Register callbacks for timer capture and timer overflow compensation.
	gpt_register_callback(ul_cap, GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_CAPTURE_EVENT);
	gpt_register_callback(overflow_compensate, GPTIMER_1, GPTIMER_SUBTIMER_B, GPTIMER_TIMEOUT_EVENT);


  	// Set up Opo Tx/Rx Control to RX
  	GPIO_SET_OUTPUT(GPIO_C_BASE, 0x01);
  	GPIO_SET_PIN(GPIO_C_BASE, 0x01);

  	// Setup and enable wakeup pin.
  	GPIO_SET_INPUT(GPIO_C_BASE, 0x80);
  	ioc_set_over(GPIO_C_NUM, 7, IOC_OVERRIDE_DIS);
  	gpio_register_callback(wake_up, GPIO_C_NUM, 7);
  	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_C);

	while(1) {
		PROCESS_YIELD();
		if (ev == PROCESS_EVENT_TIMER) {
			if(td != 0) {
				test_data[4] = td;
			}
			else {
				test_data[4] = 65;
			}
			packetbuf_clear();
			packetbuf_copyfrom((void *) test_data, 6);
			NETSTACK_MAC.send(NULL, NULL);
			td = 0;
			started = 0;
			nvic_interrupt_enable(NVIC_INT_GPIO_PORT_C); // time to listen for wake ups again
		}

	}
	PROCESS_END();
}

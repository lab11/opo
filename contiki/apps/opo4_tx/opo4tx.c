#include "opo4tx.h"
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
#include "dev/leds.h"
#include "sys/rtimer.h"

/*---------------------------------------------------------------------------*/
PROCESS(tx_test, "Opo4Tx");
PROCESS(tx_test_timer_setter, "Opo4TxTimerSetter");
AUTOSTART_PROCESSES(&tx_test, &tx_test_timer_setter);
/*---------------------s------------------------------------------------------*/

uint8_t open_pipes[64] = {0};
uint8_t test_data[6] = {65,65,65,65,65,65};
nrf8001_event_packet ep = {0};
int msg_count = 0;
uint8_t test = 0;
static struct etimer et;
uint tx_stage = 0;
void sfd_callback();
void txdone_callback();

void sfd_callback() {
	gpt_enable_event(1, GPTIMER_SUBTIMER_A);
}

void txdone_callback() {
	tx_stage = 3;
	process_poll(&tx_test);
}

PROCESS_THREAD(tx_test_timer_setter, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		etimer_set(&et, 1);
	}
	PROCESS_END();
}


PROCESS_THREAD(tx_test, ev, data) {
	PROCESS_BEGIN();

	// Setting all the radio shit up.
	packetbuf_clear();
	packetbuf_copyfrom((void *) test_data, 6);
	ble_ant_enable();

	//Setting the PWM stuff up on GPTIMER_1, SUBTIMER_A
	ungate_gpt(1);
	gpt_set_16_bit_timer(1);
	gpt_set_mode(GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_PERIODIC_MODE);
	gpt_set_alternate_mode(GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_ALTERNATE_MODE_PWM);
	gpt_set_interval_value(GPTIMER_1, GPTIMER_SUBTIMER_A, 0x190);
	gpt_set_match_value(1, GPTIMER_SUBTIMER_A, 0xC8);
	ioc_set_sel(OPO_PWM_PORT_NUM, OPO_PWM_PIN_NUM, IOC_PXX_SEL_GPT1_ICP1);
  	ioc_set_over(OPO_PWM_PORT_NUM, OPO_PWM_PIN_NUM, IOC_OVERRIDE_OE);
  	GPIO_PERIPHERAL_CONTROL(OPO_PWM_PORT_BASE, OPO_PWM_PIN_MASK);

  	// Set up Opo Tx/Rx Control to TX
  	GPIO_SET_OUTPUT(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);
  	GPIO_SET_PIN(OPO_TX_RX_SEL_PORT_BASE, OPO_TX_RX_SEL_PIN_MASK);

  	SFD_HANDLER.set_callback(sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(txdone_callback);

	etimer_set(&et, CLOCK_SECOND * 5);
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
			if (tx_stage == 0) {
				gpt_enable_event(GPTIMER_1, GPTIMER_SUBTIMER_A);
				tx_stage = 1;
				etimer_set(&et, CLOCK_SECOND/1000);
			}
			else if(tx_stage == 1) {
				gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_A);
				tx_stage = 2;
				etimer_set(&et, CLOCK_SECOND/1000 * 70);
			}
			else if (tx_stage == 2) {
				packetbuf_clear();
				packetbuf_copyfrom((void *) test_data, 6);
				SFD_HANDLER.set_callback(sfd_callback);
				NETSTACK_MAC.on();
				NETSTACK_MAC.send(NULL, NULL);
				PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
				etimer_set(&et, CLOCK_SECOND/1000);
			}
			else if (tx_stage == 3) {
				NETSTACK_MAC.off(0);
				leds_toggle(LEDS_GREEN);
				//leds_toggle(LEDS_BLUE);
				//leds_toggle(LEDS_GREEN);
				gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_A);
				tx_stage = 0;
				etimer_set(&et, CLOCK_SECOND * 5);
			}

	}
	PROCESS_END();
}

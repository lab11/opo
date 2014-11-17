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
#include "opo_helper.h"

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
uint8_t blah;

void sfd_callback() {
	gpt_enable_event(1, GPTIMER_SUBTIMER_A);
}

void txdone_callback() {
	tx_stage = 3;
	process_poll(&tx_test);
}

void rfrx_callback() {
	blah = 1;
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
	cc2538_ant_enable();

	opo_init();

  	enable_opo_ul_tx(); // Set up Opo Tx/Rx Control to TX

  	SFD_HANDLER.set_callback(sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(txdone_callback);
  	simple_network_set_callback(rfrx_callback);

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
				gpt_disable_event(GPTIMER_1, GPTIMER_SUBTIMER_A);
				tx_stage = 0;
				etimer_set(&et, CLOCK_SECOND * 5);
			}

	}
	PROCESS_END();
}

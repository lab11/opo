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
#include "opo.h"

/*---------------------------------------------------------------------------*/
PROCESS(opo4rx, "Opo4Rx");
AUTOSTART_PROCESSES(&opo4rx);
/*---------------------s------------------------------------------------------*/

static struct etimer et;

static void opo_rx_callback(bool success,
	     					uint32_t ul_rf_dt,
					 		uint32_t ul_dt,
					 		opo_rxmsg_t rxmsg) {
	if(success == true) {
		leds_toggle(LEDS_BLUE);
	} else {
		leds_toggle(LEDS_RED);
	}
	process_poll(&opo4rx);
}

PROCESS_THREAD(opo4rx, ev, data) {
	PROCESS_BEGIN();

	opo_init();

	register_opo_rx_callback(opo_rx_callback);
	enable_opo_rx();

	//cc2538_ant_enable();

	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		etimer_set(&et, (CLOCK_SECOND) );
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		enable_opo_rx();
	}


	PROCESS_END();
}

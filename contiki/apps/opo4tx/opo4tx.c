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
#include "opo.h"

/*---------------------------------------------------------------------------*/
PROCESS(tx_test, "Opo4Tx");
AUTOSTART_PROCESSES(&tx_test);
/*---------------------s------------------------------------------------------*/

static struct etimer met;
opo_rmsg_t mmsg;

void opotx_callback() {
	leds_toggle(LEDS_BLUE);
	process_poll(&tx_test);
}

PROCESS_THREAD(tx_test, ev, data) {
	PROCESS_BEGIN();

	opo_init();
	register_opo_tx_callback(opotx_callback);


	mmsg.preamble = OPO_PREAMBLE;
	mmsg.tx_id = 3;

	etimer_set(&met, CLOCK_SECOND * 5);
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		perform_opo_tx(&mmsg);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		etimer_set(&met, CLOCK_SECOND * 3);
	}

	PROCESS_END();
}

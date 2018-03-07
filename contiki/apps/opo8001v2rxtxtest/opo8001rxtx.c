#include "contiki.h"
#include "opo.h"
#include "lib/random.h"
#include "vtimer.h"
#include "simplestore.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc2538-rf-debug.h"
#include "rtc.h"

PROCESS(opo8001rxtx, "opo8001rxtx");
PROCESS(opo8001rx, "opo8001rx");
PROCESS(opo8001tx, "opo8001tx");
AUTOSTART_PROCESSES(&opo8001rxtx);
static struct vtimer tx_delay;
static opo_data_t mdata;
static bool sending_fail = false;
time_t test_time = 1496878395;

static unsigned short generate_rand_tx() {
	unsigned short r = random_rand();
	unsigned short timer = 2000 > r ? 2000 % r: r % 2000;
	return timer;
}

void opo_rx_callback(opo_data_t odata) {
	memcpy(&mdata, &odata, sizeof(opo_data_t));
	process_poll(&opo8001rx);
}

void tx_delay_callback() {
	process_poll(&opo8001tx);
}

void opo_tx_callback() {
	unsigned short delaytime = generate_rand_tx();
	schedule_vtimer(&tx_delay, 2*VTIMER_SECOND + delaytime);
	enable_opo_rx();
}


PROCESS_THREAD(opo8001rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		enable_opo_rx();
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001tx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		send_rf_debug_msg("Perform Opo TX");
		if(!perform_opo_tx()) {
			unsigned short delaytime = generate_rand_tx();
			schedule_vtimer(&tx_delay, VTIMER_SECOND + (VTIMER_SECOND/1000 * delaytime));
		} else {
			sending_fail = false;
		}
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	register_opo_rx_callback((opo_rx_callback_t) opo_rx_callback);
	register_opo_tx_callback(opo_tx_callback);
	tx_delay = get_vtimer(tx_delay_callback);
	process_start(&opo8001rx, NULL);
	process_start(&opo8001tx, NULL);
	rtc_set_unixtime(test_time);

	unsigned short randtime = generate_rand_tx();
	schedule_vtimer(&tx_delay, VTIMER_SECOND + (VTIMER_SECOND/1000 * randtime));
	enable_opo_rx();
	PROCESS_END();
}

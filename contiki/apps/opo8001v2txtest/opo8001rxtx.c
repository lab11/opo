#include "contiki.h"
#include "cloudcomm.h"
#include "opo.h"
#include "lib/random.h"
#include "vtimer.h"
#include "simplestore.h"
#include "dev/gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc2538-rf-debug.h"

PROCESS(opo8001rxtx, "opo8001rxtx");
PROCESS(opo8001tx, "opo8001tx");
AUTOSTART_PROCESSES(&opo8001rxtx);
static struct vtimer tx_delay;
static cloudcomm_meta_t cc_metadata;
static char murl[] = "http://post.gatd.io/a7ffd6e1-a973-4670-a2d3-4e79c9a4a390";
static opo_data_t mdata;
static bool sending_fail = false;

/* State variables for our plug in detector */
static bool plugged_in = false;
static bool cc_done = false;
struct vtimer plug_debouncer;
struct vtimer unplug_debouncer;

static unsigned short generate_rand_tx() {
	unsigned short r = random_rand();
	unsigned short timer = 2000 > r ? 2000 % r: r % 2000;
	return timer;
}

void init_callback() {
	process_poll(&opo8001rxtx);
}

void tx_delay_callback() {
	process_poll(&opo8001tx);
}

void opo_tx_callback() {
	unsigned short delaytime = generate_rand_tx();
	schedule_vtimer(&tx_delay, 10*VTIMER_SECOND);
}

PROCESS_THREAD(opo8001tx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		send_rf_debug_msg("Perform Opo TX");
		if(!perform_opo_tx()) {
			sending_fail = true;
			unsigned short delaytime = generate_rand_tx();
			schedule_vtimer(&tx_delay, 10*VTIMER_SECOND);
		}
		else {
			sending_fail = false;
		}
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	register_opo_tx_callback(opo_tx_callback);
	tx_delay = get_vtimer(tx_delay_callback);
	process_start(&opo8001tx, NULL);

	cc_metadata.dest = &murl[0];
	cc_metadata.dest_len = 56;

	cloudcomm_set_packet_length(sizeof(opo_data_t));
	cloudcomm_set_metainfo(&cc_metadata);
	cloudcomm_clear_data();

	cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
	cloudcomm_on(init_callback, 0);
	leds_on(LEDS_RED);

	PROCESS_YIELD_UNTIL(ev==PROCESS_EVENT_POLL); // cloudcomm has obtained the time.

	leds_off(LEDS_RED);
	unsigned short randtime = generate_rand_tx();
	schedule_vtimer(&tx_delay, 5*VTIMER_SECOND);

	PROCESS_END();
}
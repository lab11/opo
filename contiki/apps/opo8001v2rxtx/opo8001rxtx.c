#include "contiki.h"
#include "cloudcomm.h"
#include "opo.h"
#include "lib/random.h"
#include "vtimer.h"
#include "simplestore.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc2538-rf-debug.h"

PROCESS(opo8001rxtx, "opo8001rxtx");
PROCESS(opo8001rx, "opo8001rx");
PROCESS(opo8001tx, "opo8001tx");
PROCESS(opo8001ccon, "enable cloudcomm");
AUTOSTART_PROCESSES(&opo8001rxtx);
static struct vtimer tx_delay;
static cloudcomm_meta_t cc_metadata;
static char murl[] = "https://gbns1w57rtvk.runscope.net/";
static opo_data_t mdata;
static uint8_t cc_delay = 0;
static void cloudcomm_callback();
static bool sending_fail = false;

static unsigned short generate_rand_tx() {
	unsigned short r = random_rand();
	unsigned short timer = 2000 > r ? 2000 % r: r % 2000;
	return timer;
}

void init_callback() {
	process_poll(&opo8001rxtx);
}

void opo_rx_callback(opo_data_t odata) {
	memcpy(&mdata, &odata, sizeof(opo_data_t));
	send_rf_debug_msg("Opo Rx");
	char debug[100];
	snprintf(debug, 100, "Timing: %lu, time: %lu\n", mdata.range_dt, mdata.m_unixtime);
	send_rf_debug_msg(debug);
	process_poll(&opo8001rx);
}

void tx_delay_callback() {
	send_rf_debug_msg("Opo Tx Delay Callback");
	process_poll(&opo8001tx);
}

void opo_tx_callback() {
	if(++cc_delay >= 150 && !sending_fail) {
		send_rf_debug_msg("Opo Tx CC Time");
		cc_delay = 0;
		process_poll(&opo8001ccon);
	} else {
		unsigned short delaytime = generate_rand_tx();
		char buffer[50];
		snprintf(buffer, 50, "Opo Tx Go Time: %u\n", delaytime);
		send_rf_debug_msg(buffer);
		schedule_vtimer(&tx_delay, VTIMER_SECOND + (VTIMER_SECOND/1000 * delaytime));
		enable_opo_rx();
	}
}

static void cloudcomm_callback() {
	send_rf_debug_msg("Cloudcomm Done");
	opo_tx_callback();
}

PROCESS_THREAD(opo8001ccon, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		send_rf_debug_msg("Cloudcomm On");
		cloudcomm_on(cloudcomm_callback, 15000);
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		cloudcomm_store(&mdata);
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
			char buffer[50];
			snprintf(buffer, 50, "Opo Tx Failed: %u\n", delaytime);
			send_rf_debug_msg(buffer);
			sending_fail = true;
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
	process_start(&opo8001ccon, NULL);

	cc_metadata.dest = &murl[0];
	cc_metadata.dest_len = 34;
	cloudcomm_set_packet_length(sizeof(opo_data_t));
	cloudcomm_set_metainfo(&cc_metadata);
	char buffer[50];
	snprintf(buffer, 50, "Opo Data Size: %u", sizeof(opo_data_t));
	send_rf_debug_msg(buffer);
	cloudcomm_clear_data();
	cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
	cloudcomm_on(init_callback, 0);
	PROCESS_YIELD_UNTIL(ev==PROCESS_EVENT_POLL);
	unsigned short randtime = generate_rand_tx();
	schedule_vtimer(&tx_delay, VTIMER_SECOND + (VTIMER_SECOND/1000 * randtime));
	enable_opo_rx();
	PROCESS_END();
}

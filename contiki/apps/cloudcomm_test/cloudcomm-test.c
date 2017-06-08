#include "cloudcomm-test.h"
#include "dev/leds.h"
#include <stdlib.h>
#include "nrf8001.h"
#include "opo.h"
#include "simplestore.h"
#include "vtimer.h"
#include "cc2538-rf-debug.h"

PROCESS(cloudcomm_test, "CCTest");
AUTOSTART_PROCESSES(&cloudcomm_test);

static opo_data_t test[24];
static cloudcomm_meta_t m;
static char murl[] = "https://gbns1w57rtvk.runscope.net/";
static vtimer cctestvtimer;

void woot() {leds_off(LEDS_GREEN);}
static void vcallback() {process_poll(&cloudcomm_test);}

PROCESS_THREAD(cloudcomm_test, ev, data) {
	// lets us know that we can keep transmitting
	PROCESS_BEGIN();
	uint8_t i = 0;
	cloudcomm_set_packet_length(sizeof(opo_data_t));
	simplestore_clear_flash_chip();

	m.dest = &murl[0];
	m.dest_len = 34;
	cloudcomm_set_metainfo(&m);

	cctestvtimer = get_vtimer(vcallback);
	schedule_vtimer(&cctestvtimer, VTIMER_SECOND * 10);
	PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
	request_cloudcomm_data(CLOUDCOMM_REQ_TIME); 
	leds_on(LEDS_GREEN);
	for(i=0;i<24;i++) {
		test[i].version_num = 0xaabb;
		test[i].rx_id = i;
		test[i].tx_id = 0x02;
		test[i].range_dt = 0xbbccddee;
		test[i].ul_dt = 0x00;
		test[i].m_unixtime = 0;
		test[i].m_time_confidence = 1;
		test[i].failed_rx_count = 2;
		test[i].tx_unixtime = 3;
		test[i].tx_time_confidence = 4;
		test[i].ul_rf_dt = 5;
		cloudcomm_store(&test[i]);
	}

	cloudcomm_on(woot, 30000);

	PROCESS_END();
}

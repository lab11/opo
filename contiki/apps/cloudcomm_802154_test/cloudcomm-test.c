#include "dev/leds.h"
#include <stdlib.h>
#include "nrf8001.h"
#include "opo.h"
#include "vtimer.h"
#include "cc2538-rf-debug.h"
#include "cloudcomm-802154.h"

PROCESS(cloudcomm_test, "CCTest");
AUTOSTART_PROCESSES(&cloudcomm_test);

static opo_data_t test;
void woot() {leds_off(LEDS_GREEN);}

PROCESS_THREAD(cloudcomm_test, ev, data) {
	// lets us know that we can keep transmitting
	PROCESS_BEGIN();
	uint8_t i = 0;
	
	simplestore_clear_flash_chip();
	cloudcomm_set_packet_length(sizeof(opo_data_t));
	cloudcomm_init();

	leds_on(LEDS_GREEN);

	cloudcomm_request_data(CLOUDCOMM_REQ_TIME); 
	test.version_num = 0x02;
	test.rx_id = 1; 
	test.tx_id = 2;
	test.range_dt = 327;
	test.m_unixtime = 3;
	test.failed_rx_count = 4; 
	test.tx_unixtime = 5;

	for(i=0;i<36;i++) {
		cloudcomm_store(&test);
		test.range_dt += 1;
	}

	cloudcomm_on(woot);

	PROCESS_END();
}

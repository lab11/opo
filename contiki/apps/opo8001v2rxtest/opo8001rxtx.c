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
PROCESS(opo8001rx, "opo8001rx");
AUTOSTART_PROCESSES(&opo8001rxtx);
static struct vtimer tx_delay;
static cloudcomm_meta_t cc_metadata;
static char murl[] = "http://post.gatd.io/a7ffd6e1-a973-4670-a2d3-4e79c9a4a390";
static opo_data_t mdata;

void init_callback() {
	process_poll(&opo8001rxtx);
}

void opo_rx_callback(opo_data_t odata) {
	memcpy(&mdata, &odata, sizeof(opo_data_t));
	process_poll(&opo8001rx);
}



PROCESS_THREAD(opo8001rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		cloudcomm_store(&mdata);
		if(enable_opo_rx() == 0) {
			leds_on(LEDS_BLUE);
		}
	}
	PROCESS_END();
}



PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	register_opo_rx_callback((opo_rx_callback_t) opo_rx_callback);
	process_start(&opo8001rx, NULL);

	GPIO_SOFTWARE_CONTROL(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_SET_INPUT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	IOC_SET_OVER(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_NUM, IOC_OVERRIDE_DIS);

	cc_metadata.dest = &murl[0];
	cc_metadata.dest_len = 56;
	cloudcomm_set_packet_length(sizeof(opo_data_t));
	cloudcomm_set_metainfo(&cc_metadata);

	cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
	cloudcomm_on(init_callback, 0);
	leds_on(LEDS_RED);

	PROCESS_YIELD_UNTIL(ev==PROCESS_EVENT_POLL); // cloudcomm has obtained the time.
	leds_off(LEDS_RED);

	if(enable_opo_rx() == 0) {
		leds_on(LEDS_BLUE);
	}
	PROCESS_END();
}

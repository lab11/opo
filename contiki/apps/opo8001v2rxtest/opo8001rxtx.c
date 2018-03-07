#include "contiki.h"
#include "cloudcomm.h"
#include "opo.h"
#include "vtimer.h"
#include "simplestore.h"
#include "dev/gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc2538-rf-debug.h"
#include "rtc.h"

#define OPO_ID 0

PROCESS(opo8001rxtx, "opo8001rxtx");
PROCESS(opo8001rx, "opo8001rx");
AUTOSTART_PROCESSES(&opo8001rxtx);
static opo_data_t mdata;
time_t test_time = 1496878395;

struct vtimer rx_test_sleeper; // gotta have a timer going to keep mcu in sleep.

void init_callback() {
	process_poll(&opo8001rxtx) ;
}

static void opo_rx_callback(opo_data_t odata) {
	memcpy(&mdata, &odata, sizeof(opo_data_t));
	char buffer[100];
	snprintf(buffer, 100, "RX_TEST: Range time %lu", mdata.range_dt);
	send_rf_debug_msg(buffer);
	leds_toggle(LEDS_GREEN);
	process_poll(&opo8001rx);
}

PROCESS_THREAD(opo8001rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(enable_opo_rx() == false) {
			leds_on(LEDS_BLUE);
		}
	}
	PROCESS_END();
}


PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	register_opo_rx_callback((opo_rx_callback_t) opo_rx_callback);
	process_start(&opo8001rx, NULL);

	/*
	GPIO_SOFTWARE_CONTROL(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_SET_INPUT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	IOC_SET_OVER(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_NUM, IOC_OVERRIDE_DIS);
	*/
	rtc_set_unixtime(test_time);
	if(enable_opo_rx() == false) {
		//leds_on(LEDS_BLUE);
	}
	PROCESS_END();
}

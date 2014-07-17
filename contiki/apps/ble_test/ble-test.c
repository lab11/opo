#include "ble-test.h"
#include "nrf8001.h"
#include "contiki.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
PROCESS(ble_test, "BleTest");
AUTOSTART_PROCESSES(&ble_test);
/*---------------------------------------------------------------------------*/

nrf8001_event_packet ep;

PROCESS_THREAD(ble_test, ev, data) {
	PROCESS_BEGIN();
	nrf8001_init(&ep);
	leds_on(LEDS_ALL);
	while(1) {
		PROCESS_YIELD();
		if (ev == sensors_event) {
			if (data == &nrf8001_event) {
				leds_on(LEDS_ALL);
			}
		}
	}

	PROCESS_END();
}

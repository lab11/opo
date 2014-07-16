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

PROCESS_THREAD(ble_test, ev, data) {
	int i;

	PROCESS_BEGIN();

	leds_off(LEDS_ALL);

	while(1) {
		PROCESS_YIELD();
	}

	PROCESS_END();
}

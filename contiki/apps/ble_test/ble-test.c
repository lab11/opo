#include "ble-test.h"
#include "nrf8001.h"
#include "contiki.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "dev/leds.h"
#include "nrf8001.h"

/*---------------------------------------------------------------------------*/
PROCESS(ble_test, "BleTest");
AUTOSTART_PROCESSES(&ble_test);
/*---------------------------------------------------------------------------*/

nrf8001_event_packet ep;

PROCESS_THREAD(ble_test, ev, data) {
	PROCESS_BEGIN();
	nrf8001_enable();
	while(1) {
		PROCESS_YIELD();
		if(ev == sensors_event) {
			uint8_t i = 0;
			if(data == &nrf8001_event) {
				leds_on(LEDS_GREEN);
				ep = nrf8001_get_event();
				printf("Length: %h\n", ep.length);
				printf("Event: %h\n", ep.event);
				for(i=0;i<ep.length-1;i++) {
					printf("Packet %u: %h", ep.packet[i]);
				}
				if(ep.event == 0x81) {
					leds_on(LEDS_RED);
				}
			}
		}
	}

	PROCESS_END();
}

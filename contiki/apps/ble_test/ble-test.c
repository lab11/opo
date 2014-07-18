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
				if(ep.event == 0x81) {
					uint8_t p[10];
					for(i=0;i<10;i++) {
						p[i] = i;
					}
					if(ep.packet[0] == 0x02) {
						leds_on(LEDS_GREEN);
						nrf8001_test(0x02);
					}
					else if(ep.packet[0] == 0x01) {
						leds_on(LEDS_RED);
						nrf8001_echo(10, &p);
					}
				}
				else if (ep.event == 0x82) {
					leds_on(LEDS_YELLOW);
				}
			}
		}
	}

	PROCESS_END();
}

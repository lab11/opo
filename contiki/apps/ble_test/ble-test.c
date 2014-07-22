#include "ble-test.h"
#include "nrf8001.h"
#include "contiki.h"
#include "dev/leds.h"
#include "nrf8001.h"
#include "dev/gpio.h"
#include "dev/ioc.h"

/*---------------------------------------------------------------------------*/
PROCESS(ble_test, "BleTest");
AUTOSTART_PROCESSES(&ble_test);
/*---------------------s------------------------------------------------------*/

nrf8001_event_packet ep = {0};

// Function to delay startup until we pull a pin high
static void config_delay_interrupt() {
	GPIO_SOFTWARE_CONTROL(DELAY_INT_PORT_BASE, DELAY_INT_PIN_MASK);
	GPIO_SET_INPUT(DELAY_INT_PORT_BASE, DELAY_INT_PIN_MASK);
	GPIO_DETECT_EDGE(DELAY_INT_PORT_BASE, DELAY_INT_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(DELAY_INT_PORT_BASE, DELAY_INT_PIN_MASK);
	GPIO_DETECT_FALLING(DELAY_INT_PORT_BASE, DELAY_INT_PIN_MASK);
	GPIO_ENABLE_INTERRUPT(DELAY_INT_PORT_BASE, DELAY_INT_PIN_MASK);
	ioc_set_over(DELAY_INT_PORT, DELAY_INT_PIN, IOC_OVERRIDE_PUE);
	nvic_interrupt_enable(DELAY_INT_PORT);
	gpio_register_callback(nrf8001_enable, DELAY_INT_PORT, DELAY_INT_PIN);
}

PROCESS_THREAD(ble_test, ev, data) {
	PROCESS_BEGIN();
	nrf8001_enable();
	while(1) {
		PROCESS_YIELD();
		if(ev == sensors_event) {
			uint8_t i = 0;
			ep = nrf8001_get_event();
			if(data == &nrf8001_event) {
				if(ep.event == 0x81) {
					uint8_t p[10];
					for(i=0;i<10;i++) {
						p[i] = i;
					}
					if(ep.packet[0] == 0x02) {
						leds_on(LEDS_RED);
						nrf8001_test(0x02);
					}
					else if(ep.packet[0] == 0x01) {
						leds_on(LEDS_YELLOW);
						nrf8001_echo(10, &p);
					}
				}
				else if (ep.event == 0x82) {
					leds_on(LEDS_GREEN);
				}
			}
		}
	}

	PROCESS_END();
}

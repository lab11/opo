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
nrf8001_setup_msg_t setup[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;
int msg_count = 0;
uint8_t test = 0;
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
						if(msg_count < NB_SETUP_MESSAGES) {
							nrf8001_setup(setup[msg_count++]);
						}
					}
					else if(ep.packet[0] == 0x03) {
						if(test > 0) {
							leds_on(LEDS_YELLOW);
							leds_on(LEDS_RED);
						}
						test++;
						nrf8001_connect(0, 32);
					}
				}
				else if (ep.event == 0x84) {
					if(ep.packet[0] == 0x06) {
						if(ep.packet[1] == 0x01) {
							if(msg_count < NB_SETUP_MESSAGES) {
								leds_on(LEDS_GREEN);
								nrf8001_setup(setup[msg_count++]);
							}
						}
					}
				}
			}
		}
	}

	PROCESS_END();
}

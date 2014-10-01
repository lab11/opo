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

uint8_t open_pipes[64] = {0};
uint8_t test_data[6] = {65,65,65,65,65,65};
nrf8001_event_packet ep = {0};
int msg_count = 0;
uint8_t test = 0;
static struct etimer et;

void led_green_on() {
	leds_on(LEDS_GREEN);
}

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
	gpio_register_callback(led_green_on, DELAY_INT_PORT, DELAY_INT_PIN);
}

PROCESS_THREAD(ble_test, ev, data) {
	PROCESS_BEGIN();

	//GPIO_SOFTWARE_CONTROL(RADIO_SELECT_PORT, RADIO_SELECT_PIN);
	//GPIO_SET_OUTPUT(RADIO_SELECT_PORT, RADIO_SELECT_PIN);
	//GPIO_CLR_PIN(RADIO_SELECT_PORT, RADIO_SELECT_PIN);

	config_delay_interrupt();
	nrf8001_enable();
	while(1) {
		PROCESS_YIELD();
		if(ev == sensors_event) {
			if(data == &nrf8001_event) {
				ep = nrf8001_get_event();
				if(ep.event == NRF8001_DEVICE_STARTED_EVENT) {
					if(ep.packet[0] == 0x02) {
						nrf8001_setup();
					}
					else if(ep.packet[0] == 0x03) {
						leds_on(LEDS_RED);
						//nrf8001_bond(180, 32);
						nrf8001_connect(0, 32);
					}
				}
				else if(ep.event == NRF8001_PIPE_STATUS_EVENT) {
					uint8_t pos = 0;
					uint8_t bit_count = 0;
					uint8_t byte_count = 0;
					uint8_t i = 0;

					for(i=0; i < 64; i++) {
						open_pipes[i] = 0;
					}
					for(byte_count=0;byte_count<8;byte_count++) {
						uint8_t filter = 1;
						for(bit_count=0;bit_count<8;bit_count++) {
							if(ep.packet[byte_count] & filter) {
								pos = bit_count * (byte_count+1);
								open_pipes[pos] = 1;
							}
							filter *= 2;
						}
					}

					etimer_set(&et, CLOCK_SECOND*5);

				}
				else if(ep.event == NRF8001_DISCONNECTED_EVENT) {

				}
			}
		}
		else if (ev == PROCESS_EVENT_TIMER) {
			if (open_pipes[PIPE_OPOBLETEST_OPOTEST_TX] == 1) {
				uint8_t i = 0;
				nrf8001_send_data(PIPE_OPOBLETEST_OPOTEST_TX,
								  6,
								  &test_data);
				for(i=0;i<6;i++) {
					test_data[i] += 1;
				}

				if(test_data[0] < 89) {
					etimer_set(&et, CLOCK_SECOND*5);
				} else {
					//leds_on(LEDS_GREEN);
				}
			}
			else {
				//leds_on(LEDS_YELLOW);
			}
		}
	}

	PROCESS_END();
}

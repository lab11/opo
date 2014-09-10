#include "rf-test.h"
#include "nrf8001.h"
#include "contiki.h"
#include "dev/leds.h"
#include "nrf8001.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/cc2538-rf.h"
#include "dev/radio.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(rf_test, "BleTest");
AUTOSTART_PROCESSES(&rf_test);
/*---------------------s------------------------------------------------------*/

uint8_t open_pipes[64] = {0};
uint8_t test_data[6] = {65,65,65,65,65,65};
nrf8001_event_packet ep = {0};
int msg_count = 0;
uint8_t test = 0;
static struct etimer et;

void rf_rx_handler() {
	uint8_t *packet_ptr = packetbuf_dataptr();
	uint16_t packet_length = packetbuf_datalen();
	uint16_t i = 0;

	printf("0x");

	for(i=0; i < packet_length; i++) {
		printf("%x", packet_ptr[i]);
	}

	printf("\n");
}

PROCESS_THREAD(rf_test, ev, data) {
	PROCESS_BEGIN();
	//packetbuf_clear();
	//packetbuf_copyfrom((void *) test_data, 6);
	simple_network_set_callback(&rf_rx_handler);
	NETSTACK_MAC.on();
	leds_on(LEDS_RED);
	/*
	etimer_set(&et, CLOCK_SECOND * 5);
	while(1) {
		PROCESS_YIELD();
		leds_toggle(LEDS_RED);
		if (ev == PROCESS_EVENT_TIMER) {
			//packetbuf_clear();
			//packetbuf_copyfrom((void *) test_data, 6);
			NETSTACK_MAC.send(NULL, NULL);
			etimer_set(&et, CLOCK_SECOND * 5);
		}
	} */
	PROCESS_END();
}

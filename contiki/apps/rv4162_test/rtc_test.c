#include "contiki.h"
#include "dev/i2c.h"
#include "rv4162.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"

#define CC2538_RF_CONF_CHANNEL 21

PROCESS(rtc_test, "rtc-test");
AUTOSTART_PROCESSES(&rtc_test);

uint8_t write_buffer[8] = {0};
uint8_t read_buffer[8] = {0};
uint8_t i = 0;

void sfd_callback() {
	uint8_t b = 0;
}

void txdone_callback() {
	uint8_t c = 0;
}

void rfdone_callback() {
	uint8_t d = 0;
}

static struct etimer et;

PROCESS_THREAD(rtc_test, ev, data) {
	PROCESS_BEGIN();
	write_buffer[0] = 0; // subsec
    write_buffer[1] = 0; // seconds
    write_buffer[2] = 44; // Minutes
    write_buffer[3] = 20; // Hours
    write_buffer[4] = 4;  // Day
    write_buffer[5] = 15;   // Date
    write_buffer[6] = 10;   // Month
    write_buffer[7] = 14;   // Year

    rv4162_set_time(&write_buffer[0]);
    SFD_HANDLER.set_callback(sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(txdone_callback);
  	simple_network_set_callback(rfdone_callback);

    NETSTACK_MAC.on();


	while(1) {
		etimer_set(&et, CLOCK_SECOND * 3);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		rv4162_read_full_time(&read_buffer[0]);
		packetbuf_clear();
		packetbuf_copyfrom((void *) read_buffer, 8);
		NETSTACK_MAC.send(NULL, NULL);
	}

	PROCESS_END();
}
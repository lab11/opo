#include "contiki.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "sst25vf.h"
#include "flash-test.h"

#define CC2538_RF_CONF_CHANNEL 21

PROCESS(flash_test, "flash-test");
AUTOSTART_PROCESSES(&flash_test);

uint32_t page_count = 1;
uint8_t stage = 0;

opo_sunit_t mbuf[12] = {0};
opo_sunit_t rmbuf[12] = {0};

void sfd_callback() {
	uint8_t b = 0;
}

void txdone_callback() {
	uint8_t c = 0;
}

void rfdone_callback() {
	uint8_t d = 0;
}

void chip_erase_callback() {
	stage = 1;
	process_poll(&flash_test);
}

void flash_program_callback() {
	stage = 2; 
	process_poll(&flash_test);
}

PROCESS_THREAD(flash_test, ev, data) {
	PROCESS_BEGIN();
	uint8_t i = 0;
	for(i=0;i<12;i++) {
		mbuf[i].rx_id = 27;
	}

    SFD_HANDLER.set_callback(sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(txdone_callback);
  	simple_network_set_callback(rfdone_callback);

    NETSTACK_MAC.on();


    sst25vf_turn_on();
    sst25vf_chip_erase();

    while(1) {
    	PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
    	if(stage == 1) {
    		sst25vf_program(page_count*256, (void*) &mbuf, sizeof(mbuf));
    	}
    	else if(stage == 2) {
    		sst25vf_read_page(page_count*256, (void*) &rmbuf, sizeof(rmbuf));
    	}
    }


    /*
	while(1) {
		etimer_set(&et, CLOCK_SECOND * 3);
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		packetbuf_clear();
		packetbuf_copyfrom((void *) read_buffer, 8);
		NETSTACK_MAC.send(NULL, NULL);
	} */

	PROCESS_END();
}
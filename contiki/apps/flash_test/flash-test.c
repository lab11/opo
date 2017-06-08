#include "contiki.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "sst25vf.h"
#include "flash-test.h"
#include "dev/leds.h"
#include "cloudcomm.h"
#include <stdbool.h>
#include "sys/clock.h"

#define CC2538_RF_CONF_CHANNEL 21

PROCESS(flash_test, "flash-test");
PROCESS(flash_flash, "flashflash");
AUTOSTART_PROCESSES(&flash_test);

uint32_t page_count = 0;
uint8_t stage = 0;
uint8_t stat;

uint8_t wbuf[120] = {0};
uint8_t rbuf[120] = {0};
static struct etimer ftet;

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
	if(stage == 0) {
		stage = 1;
	}
	process_poll(&flash_flash);
}

void flash_program_callback() {
	stage = 2;
	process_poll(&flash_flash);
}

PROCESS_THREAD(flash_flash, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(stage == 1) {
			sst25vf_write_status_register(0);
			sst25vf_program(page_count * 256, (void *) &wbuf, sizeof(wbuf));
		}
		else if(stage == 2) {
			uint8_t i = 0;
			bool meh = true;
			sst25vf_read_page(page_count * 256, (void *) &rbuf, sizeof(rbuf));
			for(i=0;i<120;i++) {
				if(rbuf[i] != wbuf[i]) {
					meh = false;
				}
			}
			if(meh == true) {
				leds_on(LEDS_RED);
			}
			stage = 3;
			sst25vf_chip_erase();
		}
		else if(stage == 3) {
			// We show that erase doesn't just reset the erase bit, also resets the data.
			leds_on(LEDS_GREEN);
			uint8_t i = 0;
			bool meh = true;
			sst25vf_read_page(page_count * 256, (void *) &rbuf, sizeof(rbuf));
			for(i=0;i<120;i++) {
				if(rbuf[i] != wbuf[i]) {
					meh = false;
				}
			}
			if(meh == true) {
				leds_on(LEDS_BLUE);
			}
		}
	}
	PROCESS_END();
}

PROCESS_THREAD(flash_test, ev, data) {
	PROCESS_BEGIN();
	uint8_t i = 0;
	for(i=0;i<120;i++) {
		wbuf[i] = i;
	}

    SFD_HANDLER.set_callback(sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(txdone_callback);
  	simple_network_set_callback(rfdone_callback);

    NETSTACK_MAC.on();

  	process_start(&flash_flash, NULL);
  	sst25vf_init();
  	sst25vf_set_chip_erase_callback(chip_erase_callback);
  	sst25vf_set_program_callback(flash_program_callback);
    sst25vf_turn_on();

    sst25vf_write_status_register(0);
    sst25vf_write_enable();

    sst25vf_chip_erase();

	PROCESS_END();
}
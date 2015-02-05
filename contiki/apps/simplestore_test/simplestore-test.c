#include "contiki.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "dev/leds.h"
#include "cloudcomm.h"
#include <stdbool.h>
#include "sys/clock.h"
#include "simplestore.h"

#define CC2538_RF_CONF_CHANNEL 21

PROCESS(flash_test, "flash-test");
PROCESS(flash_flash, "flashflash");
AUTOSTART_PROCESSES(&flash_test);

static uint8_t wbuf[120] = {0};
static uint8_t rbuf[120] = {0};

static void erase_callback() {
	simplestore_write_next_page(&wbuf, 120);
}

static void program_callback() {
	uint8_t i = 0;
	bool meh = true;
	leds_on(LEDS_RED);
	simplestore_read_next_page(&rbuf, 120);
	for(i=0;i<120;i++) {
		if(rbuf[i] != wbuf[i]) {
			meh = false;
			break;
		}
	}
	if(meh) {
		leds_on(LEDS_BLUE);
	}
	else {
		leds_on(LEDS_GREEN);
	}
}

PROCESS_THREAD(flash_flash, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);

	}
	PROCESS_END();
}

PROCESS_THREAD(flash_test, ev, data) {
	PROCESS_BEGIN();
	uint8_t i = 0;
	for(i=0;i<120;i++) {
		wbuf[i] = i;
	}

  	process_start(&flash_flash, NULL);
  	simplestore_set_flash_erase_callback(erase_callback);
  	simplestore_set_flash_program_callback(program_callback);
  	simplestore_turn_on_flash();

  	simplestore_config();
  	simplestore_clear_flash_chip();

	PROCESS_END();
}
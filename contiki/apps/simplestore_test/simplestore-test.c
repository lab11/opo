#include "contiki.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "dev/leds.h"
#include "cloudcomm.h"
#include <stdbool.h>
#include "sys/clock.h"
#include "simplestore.h"
#include "vtimer.h"
#include "opo.h"

#define CC2538_RF_CONF_CHANNEL 21

PROCESS(flash_test, "flash-test");
AUTOSTART_PROCESSES(&flash_test);

static uint8_t wbuf[140] = {0};
static uint8_t rbuf[140] = {0};

static vtimer vt;
static void vtcallback() {
	process_poll(&flash_test);
}

PROCESS_THREAD(flash_test, ev, data) {
	PROCESS_BEGIN();
	uint8_t i = 0;
	bool tester = true;
	for(i=0;i<120;i++) {
		wbuf[i] = i;
    rbuf[i] = 0;
	}
	vt = get_vtimer(vtcallback);
  simplestore_clear_flash_chip();

  uint8_t w = simplestore_write_next_page(&wbuf[0], 120);
  uint8_t r = simplestore_read_next_page(&rbuf[0], 120);

  for(i=0;i<120;i++) {
  	if(wbuf[i] != rbuf[i]) {
  		tester = false;
  		break;
  	}
  }

  if(tester) {
  	leds_on(LEDS_GREEN);
  } else {
    leds_on(LEDS_RED);
  }

	PROCESS_END();
}

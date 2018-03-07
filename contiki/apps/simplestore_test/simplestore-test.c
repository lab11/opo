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
#include "cc2538-rf-debug.h"
#include "sst25vf.h"


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
  char buffer[100];
	for(i=0;i<120;i++) {
		wbuf[i] = i+3;
    rbuf[i] = 0;
	}
	vt = get_vtimer(vtcallback);

  simplestore_turn_off_flash();
  simplestore_turn_on_flash(); 

  /* 
  sst25vf_clear_all_block_protection();

  uint8_t bp_buf[20];
  sst25vf_read_block_protection_register(bp_buf);

  uint8_t ptr = 0;
  for( i = 0; i < 18; i++)
  {
    ptr += snprintf(buffer + ptr, sizeof(buffer) - ptr, "%u ", bp_buf[i]);   
  }
  send_rf_debug_msg(buffer);
  */
  simplestore_clear_flash_chip();

  uint8_t w = simplestore_write_next_page(wbuf, 120);
  uint8_t r = simplestore_read_next_page(rbuf, 120);

  
  /*for(i=0;i<120;i++) {
    snprintf(buffer, 100, "SS_TEST RX: %u", rbuf[i]);
    send_rf_debug_msg(buffer);
  }*/ 
  for(i=0;i<120;i++) {
  	if(wbuf[i] != rbuf[i]) {
  		tester = false;
  		break;
  	}
  }

  if(tester) {
  	leds_on(LEDS_GREEN);
    leds_on(LEDS_BLUE);
  } else {
    leds_on(LEDS_RED);
  }

  simplestore_clear_flash_chip();
	PROCESS_END();
}

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

opo_data_t test[12];
opo_data_t check[12];

PROCESS_THREAD(flash_test, ev, data) {
	PROCESS_BEGIN();
	uint8_t i = 0;

  simplestore_clear_flash_chip();

  for(i=0;i<12;i++) {
    test[i].version_num = 0xaabb;
    test[i].rx_id = i;
    test[i].tx_id = 0x02;
    test[i].range_dt = 0xbbccddee;
    test[i].ul_dt = 0x00;
    test[i].m_unixtime = 0;
    test[i].m_time_confidence = 1;
    test[i].failed_rx_count = 2;
    test[i].tx_unixtime = 3;
    test[i].tx_time_confidence = 4;
    test[i].ul_rf_dt = 5;
    check[i].version_num = 0;
  }


  simplestore_write_next_page((void *) &test[0], sizeof(opo_data_t) * 6);
  simplestore_write_next_page((void *) &test[6], sizeof(opo_data_t) * 6);
  simplestore_read_next_page((void *) &check[0], sizeof(opo_data_t) * 6);
  simplestore_read_next_page((void *) &check[6], sizeof(opo_data_t) * 6);
  for(i=0;i<12;i++) {
    if(check[i].version_num != test[i].version_num) {
      leds_on(LEDS_GREEN);
    }
    if(check[i].version_num != 0) {
      leds_on(LEDS_RED);
    }
    if(check[i].version_num == test[i].version_num) {
      leds_on(LEDS_BLUE);
    }
  }
	PROCESS_END();
}

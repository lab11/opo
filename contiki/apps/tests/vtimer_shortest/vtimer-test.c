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

static vtimer vt_short;
static vtimer vt_shorter;
static vtimer vt_shorterer;

static void vt_short_callback() {
	leds_on(LEDS_GREEN);
  schedule_vtimer_ms(&vt_shorter, 10);
}

static void vt_shorter_callback() {
  leds_on(LEDS_RED);
  schedule_vtimer(&vt_shorterer, 51);
}

static void vt_shorterer_callback() {
  leds_on(LEDS_BLUE);
}

PROCESS_THREAD(flash_test, ev, data) {
	PROCESS_BEGIN();
	vt_short = get_vtimer(vt_short_callback);
  vt_shorter = get_vtimer(vt_shorter_callback);
  vt_shorterer = get_vtimer(vt_shorterer_callback);

  schedule_vtimer_ms(&vt_short, 20);
	PROCESS_END();
}

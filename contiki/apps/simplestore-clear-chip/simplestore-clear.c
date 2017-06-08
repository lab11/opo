#include "dev/leds.h"
#include <stdlib.h>
#include "contiki.h"
#include "simplestore.h"

PROCESS(simplestore_clear, "clear");
AUTOSTART_PROCESSES(&simplestore_clear);

PROCESS_THREAD(simplestore_clear, ev, data) {
	PROCESS_BEGIN();
	simplestore_clear_flash_chip();
	leds_on(LEDS_BLUE);
	PROCESS_END();
}
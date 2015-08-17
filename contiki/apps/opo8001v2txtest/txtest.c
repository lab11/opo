#include "contiki.h"
#include "cloudcomm.h"
#include "opo.h"
#include "lib/random.h"
#include "vtimer.h"
#include "simplestore.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc2538-rf-debug.h"

PROCESS(randtest, "randtest");
AUTOSTART_PROCESSES(&randtest);

PROCESS_THREAD(randtest, ev, data) {
	PROCESS_BEGIN();
    uint8_t i = 0;
    for(i=0; i < 100; i++) {
        unsigned short t = random_rand();
        char buffer[50];
        snprintf(buffer, 50, "Rand Time: %u\n", t);
        send_rf_debug_msg(buffer);
    }

	PROCESS_END();
}

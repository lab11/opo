#include "contiki.h"
#include "cloudcomm.h"
#include "opo.h"
#include "lib/random.h"
#include "vtimer.h"
#include "simplestore.h"
#include "dev/gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc2538-rf-debug.h"

PROCESS(opo8001rxtx, "opo8001rxtx");
AUTOSTART_PROCESSES(&opo8001rxtx);


PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	NETSTACK_MAC.on();

	PROCESS_END();
}

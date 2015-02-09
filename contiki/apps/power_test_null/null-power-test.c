#include <stdlib.h>
#include "contiki.h"


PROCESS(power_test, "powerTest");
AUTOSTART_PROCESSES(&power_test);

PROCESS_THREAD(power_test, ev, data) {
	PROCESS_BEGIN();
	PROCESS_END();
}
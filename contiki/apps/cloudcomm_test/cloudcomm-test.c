#include "cloudcomm-test.h"
#include "dev/leds.h"


PROCESS(cloudcomm_test, "CCTest");
AUTOSTART_PROCESSES(&cloudcomm_test);


PROCESS_THREAD(cloudcomm_test, ev, data) {
	// lets us know that we can keep transmitting
	PROCESS_BEGIN();
	leds_on(LEDS_RED);
	PROCESS_END();
}
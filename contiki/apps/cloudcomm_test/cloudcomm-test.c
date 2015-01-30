#include "cloudcomm-test.h"
#include "dev/leds.h"
#include <stdlib.h>
#include "nrf8001.h"

PROCESS(cloudcomm_test, "CCTest");
AUTOSTART_PROCESSES(&cloudcomm_test);
static uint8_t meh[4] = {74, 65, 78, 0};

PROCESS_THREAD(cloudcomm_test, ev, data) {
	// lets us know that we can keep transmitting
	PROCESS_BEGIN();
	nrf8001_reset();
	start_cloudcomm();
	send_cloudcomm_data(&meh, 4);
	PROCESS_END();
}
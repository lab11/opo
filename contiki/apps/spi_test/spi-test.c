#include "spi-test.h"
#include "contiki.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
PROCESS(spi_test, "spiTest");
AUTOSTART_PROCESSES(&spi_test);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(spi_test, ev, data) {
	PROCESS_BEGIN();
	spi_write_test();
	while(1) {
		PROCESS_YIELD();
	}

	PROCESS_END();
}

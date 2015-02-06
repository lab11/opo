#include <stdlib.h>
#include "contiki.h"
#include "dev/systick.h"
#include "rv4162.h"

PROCESS(power_test, "powerTest");
AUTOSTART_PROCESSES(&power_test);

PROCESS_THREAD(power_test, ev, data) {
	PROCESS_BEGIN();
	rv4162_disable_clkout();
	REG(SYSTICK_STCTRL) &= ~SYSTICK_STCTRL_INTEN;

	PROCESS_END();
}
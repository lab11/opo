
#include "contiki.h"
#include "dev/leds.h"
#include "cloudcomm.h"
#include "dev/gpio.h"
#include "net/netstack.h"

/*---------------------------------------------------------------------------*/
PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blink_process, ev, data) {
	PROCESS_BEGIN();
	leds_on(LEDS_BLUE);
	NETSTACK_MAC.off(0);
	PROCESS_END();
}

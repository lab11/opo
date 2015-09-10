
#include "contiki.h"
#include "dev/leds.h"
#include "cloudcomm.h"
#include "dev/gpio.h"
#include "net/netstack.h"
#include "ioc.h"
#include "lpm.h"
#include "dev/sys-ctrl.h"
#include "dev/scb.h"
#include "rv4162.h"

/*---------------------------------------------------------------------------*/
PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blink_process, ev, data) {
	PROCESS_BEGIN();
	NETSTACK_MAC.off(0);
	PROCESS_END();
}

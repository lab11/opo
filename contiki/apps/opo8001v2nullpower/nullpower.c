
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
#include "vtimer.h"

/*---------------------------------------------------------------------------*/
PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);

static struct vtimer wootime;

static void wootime_callback() {schedule_vtimer_ms(&wootime, 60000);}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blink_process, ev, data) {
	PROCESS_BEGIN();
	NETSTACK_MAC.off(0);
	wootime = get_vtimer(wootime_callback);
	schedule_vtimer_ms(&wootime, 60000);
	PROCESS_END();
}

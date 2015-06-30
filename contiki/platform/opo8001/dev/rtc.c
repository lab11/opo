#include "rtc.h"
#include "vtimer.h"
#include "cpu.h"

static time_t base_unixtime = 0;
static uint32_t base_time = 0;
time_t rtc_get_unixtime() {
    if(base_unixtime == 0) {
        return 0;
    } else {
        return ((vtimer_now() - base_time)/32768 + base_unixtime);
    }
}

void rtc_set_unixtime(time_t btime) {
    base_unixtime = btime;
    base_time = vtimer_now();
}

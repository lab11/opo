#include "rtc.h"
#include "rv4162.h"

time_t rtc_get_unixtime() {
	return rv4162_get_unixtime();
}

void rtc_set_unixtime(time_t btime) {
	rv4162_set_unixtime(btime);
}

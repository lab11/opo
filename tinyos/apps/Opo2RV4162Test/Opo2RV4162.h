#ifndef OPO_RV4162_TEST
#define OPO_RV4162_TEST
#define CC2420_DEF_CHANNEL 16

typedef nx_struct opo_rtc_msg {
	nx_uint16_t full_time[8];
} opo_rtc_msg_t;

enum {
	OPO_RTC_TEST = 29
};

#endif
#include "blink.h"
#include "vtimer.h"
#include "leds.h"

struct vtimer blink_red_vtimer;
struct vtimer blink_blue_vtimer;
struct vtimer blink_green_vtimer;

static void blink_red_callback() { leds_off(LEDS_RED); }
static void blink_blue_callback() { leds_off(LEDS_BLUE); }
static void blink_green_callback() { leds_off(LEDS_GREEN); }

void blink_red() {
	leds_on(LEDS_RED);
	schedule_vtimer_ms(&blink_red_vtimer, 100);
}

void blink_blue() {
	leds_on(LEDS_BLUE);
	schedule_vtimer_ms(&blink_blue_vtimer, 100);
}

void blink_green() {
	leds_on(LEDS_GREEN);
	schedule_vtimer_ms(&blink_green_vtimer, 100);
}


void blink_init() {
	blink_red_vtimer = get_vtimer(blink_red_callback);
	blink_blue_vtimer = get_vtimer(blink_blue_callback);
	blink_green_vtimer = get_vtimer(blink_green_callback);
}
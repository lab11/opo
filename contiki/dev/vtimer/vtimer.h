#ifndef VTIMER_H
#define VTIMER_H

#include "contiki.h"
#include <stdint.h>
#include <stdbool.h>
#include "vtimer-arch.h"

#define VTIMER_SECOND VTIMER_ARCH_SECOND
#define VTIMER_NOW    vtimer_arch_now

typedef uint32_t vtimer_clock_t;

typedef struct vtimer {
	void (*callback)();
	uint32_t scheduled_time;
	uint32_t ticks;
	bool is_set;
	bool is_scheduled;
	struct vtimer *next;
	struct vtimer *prev;
	uint8_t debug_counter;
} vtimer;

vtimer get_vtimer(void *callback);
void schedule_vtimer(vtimer *v, uint32_t ticks); // Schedule a timer for x ticks in the future
void schedule_high_priority(vtimer *v, uint32_t ticks); // Schedules this timer ahead of all others.
void cancel_vtimer(vtimer *v); // cancel vtimer associated with a callback
void vtimer_init();
void vtimer_run_next();
uint32_t vtimer_now();

#endif
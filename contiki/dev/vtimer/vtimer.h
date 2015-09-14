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
	void (*callback)(); // callback function called on vtimer trigger
	uint32_t scheduled_time; // hardware time the vtimer should execute at. 0 if vtimer is not in the queue
	bool is_set; // Checks that this vt is the next to run. Guards against vtimer_cancel edge cases
	bool in_queue; // Checks if this vt is already in the queue when scheduling.
	struct vtimer *next;
	struct vtimer *prev;
} vtimer;

vtimer get_vtimer(void *callback);
void schedule_vtimer(vtimer *v, uint32_t ticks); // Schedule a timer for x ticks in the future
void schedule_vtimer_ms(vtimer *v, uint32_t time); // Schedule a timer for x ms in the future
void cancel_vtimer(vtimer *v); // cancel vtimer associated with a callback
void vtimer_init();
void vtimer_run_next();
uint32_t vtimer_now();

#endif

#include "vtimer.h"
#include "vtimer-arch.h"
#include "cpu.h"
#include <stdbool.h>
#include "dev/leds.h"

/**
   Priority queue based timers which are multiplexed off one hardware timer provided by VTIMER_ARCH
   Priority queue is organized by soonest to farthest time and implemented as a doubly linked list. Doesn't scale super
   well, but seriously how many timers are you going to need?

   Scheduling vtimers should be done in processes, not other interrupt contexts.

   Timer callbacks run inside interrupt contexts, so they should quickly do something and return. Do any major work
   inside of polled processes.
*/

static vtimer *head;

/* Insert a vtimer into our queue based on it's scheduled_time */
static void insert_vtimer(vtimer *v) {
	INTERRUPTS_DISABLE();

	/* Queue is empty, so we initalize the queue to v */
	if(head == NULL) {
		head = v;
		head->is_set = true;
		vtimer_arch_schedule(head->scheduled_time);
		INTERRUPTS_ENABLE();
		return;
	}

	/* The queue already exists, so iterate through and insert v */
	vtimer *iterator = head;
	while(1) {
		/* We have found where v belongs in the queue */
		if(v->scheduled_time < iterator->scheduled_time) {
			vtimer *p = iterator->prev;
			iterator->prev = v;
			v->next = iterator;
			/* We are inserting v into the middle of the queue somewhere */
			if(p != NULL) {
				v->prev = p;
				p->next = v;
			}
			/* v is the new head of the queue */
			else {
				head->is_set = false; // current head is no longer the head
				head = v; // update head
				head->is_set = true; // Update state of new head.
		 		vtimer_arch_schedule(head->scheduled_time); // schedule new head
			}
			break;
		}
		/* We have reached the end of the queue */
		if(iterator->next == NULL) {
			iterator->next = v;
			v->prev = iterator;
			break;
		}
		iterator = iterator->next;
	}

	INTERRUPTS_ENABLE();
}

/* Returns a vtimer with default values set. This is how users initialize a vtimer */
vtimer get_vtimer(void *c) {
	vtimer temp;
	temp.callback = c;
	temp.scheduled_time = 0;
	temp.is_set = false;
	temp.in_queue = false;
	temp.next = NULL;
	temp.prev = NULL;
	return temp;
}

uint32_t vtimer_now() {
	return vtimer_arch_now();
}

/* Schedules a vtimer to be run ms milliseconds in the future. */
/* If v is already in the queue, we reschedule it.             */
void schedule_vtimer_ms(vtimer *v, uint32_t ms) {
	INTERRUPTS_DISABLE();
	/* If vtimer is already in queue, cancel it before rescheduling it */
	if(v->scheduled_time != 0) { cancel_vtimer(v); }

	/* Determine when the vtimer should be run in clock ticks */
	v->scheduled_time = vtimer_arch_now() + (VTIMER_SECOND/1000 * ms);
	v->in_queue = true;

	/* Insert the vtimer into the queue */
	insert_vtimer(v);

	INTERRUPTS_ENABLE();
}

/*
 * Schedules a vtimer to be run at a later time.
 * If the vtimer is already scheled, we reschedule it.
*/
void schedule_vtimer(vtimer *v, uint32_t ticks) {
	INTERRUPTS_DISABLE();
	/* If vtimer is already in queue, cancel it before rescheduling it */
	if(v->scheduled_time != 0) { cancel_vtimer(v); }

	/* Determine when the vtimer should be run in clock ticks */
	v->scheduled_time = vtimer_arch_now() + ticks;
	v->in_queue = true;

	/* Insert the vtimer into the queue */
	insert_vtimer(v);

	INTERRUPTS_ENABLE();
}

/* Cancels a vtimer by removing it from the vtimer queue. Will not stop a vtimer mid exection. */
void cancel_vtimer(vtimer *v) {
	INTERRUPTS_DISABLE();

	/* If we are canceling the head timer, reschedule the head */
	if(head == v) {
		head = v->next;
		if(head != NULL) {
			head->is_set = true;
			vtimer_arch_schedule(head->scheduled_time);
		}
		else { vtimer_arch_cancel(); }
	}

	/* Fix the next later vtimer */
	if(v->next != NULL) { v->next->prev = v->prev; }

	/* Fix the next earlier vtimer */
	if(v->prev != NULL) { v->prev->next = v->next; }

	/* Reset canceled vtimer values */
	v->is_set = false;
	v->in_queue = false;
	v->scheduled_time = 0;
	v->next = NULL;
	v->prev = NULL;

	INTERRUPTS_ENABLE();
}


/*
 * This function is where vtimer callbacks are exectued. This is called directly from the hardware
 * timer's ISR, and handles running the vtimer callback and scheduling the next vtimer.
 * We assume that there is no interrupt priority hierarchy, meaning we don't need to explicitly
 * enable and disable interrupts.
 * We also check that the current head should actually be run to guard against cancel_vtimer edge cases.
*/
void vtimer_run_next() {
	INTERRUPTS_DISABLE();
	if(head != NULL && head->is_set) {
		vtimer *current = head;
		head = head->next;
		current->is_set = false;
		current->in_queue = false;
		current->scheduled_time = 0;
		current->prev = NULL;
		current->next = NULL;
		(*(current->callback))();
		if(head != NULL && !head->is_set) {
			head->is_set = true;
			head->prev = NULL;
			vtimer_arch_schedule(head->scheduled_time);
		}
	}
	INTERRUPTS_ENABLE();
}


void vtimer_init() {
	head = NULL;
	vtimer_arch_init();
}

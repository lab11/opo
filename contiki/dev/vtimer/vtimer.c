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

static void insert_vtimer(vtimer *v) {
	INTERRUPTS_DISABLE();
	// Nothing is currently scheduled. Current vtimer becomes the head and is scheduled.
	if(head == NULL) {
		head = v;
		head->is_scheduled = true;
		vtimer_arch_schedule(head->scheduled_time, head->ticks);
		INTERRUPTS_ENABLE();
		return;
	}
	// Other vtimers are scheduled. We iterate through them from soonest to farthest timewise to
	//
	vtimer *iterator = head;
	while(1) {
		if(v->scheduled_time < iterator->scheduled_time) { // Check to see if v should be scheduled before current vtimer.
			vtimer *p = iterator->prev;
			iterator->prev = v;
			v->next = iterator;
			if(p != NULL) {
				v->prev = p;
				p->next = v;
			} else { // v is the new head
				head->is_scheduled = false; // set current head to unscheduled.
				head = v; // update head
				head->is_scheduled = true; // Update state of new head.
		 		vtimer_arch_schedule(head->scheduled_time, head->ticks); // schedule new head
				INTERRUPTS_ENABLE();
			}
			break;
		}
		if(iterator->next == NULL) {
			iterator->next = v;
			v->prev = iterator;
			break;
		}
		iterator = iterator->next;
	}
	INTERRUPTS_ENABLE();
}

vtimer get_vtimer(void *c) {
	vtimer temp;
	temp.callback = c;
	temp.scheduled_time = 0;
	temp.is_set = false;
	temp.next = NULL;
	temp.prev = NULL;
	temp.is_scheduled = false;
	temp.ticks = 0;
	temp.debug_counter = 0;
	return temp;
}

uint32_t vtimer_now() {
	return vtimer_arch_now();
}

void schedule_vtimer(vtimer *v, uint32_t ticks) {
	if(v->scheduled_time != 0) {
		cancel_vtimer(v);
	}
	v->is_set = true;
	v->ticks = ticks;
	v->scheduled_time = vtimer_arch_now() + ticks;
	insert_vtimer(v);
}

void schedule_high_priority(vtimer *v, uint32_t ticks)  {
	INTERRUPTS_DISABLE();
	v->is_set = true;
	v->ticks = ticks;
	v->next = head;
	head->prev = v;
	head = v;
	vtimer_arch_schedule(head->scheduled_time, head->ticks);
	INTERRUPTS_ENABLE();

}

void cancel_vtimer(vtimer *v) {
	INTERRUPTS_DISABLE();
	// Check to see if we are canceling the head timer.
	// If we are and there are other timers, schedule the next head. VTIMER_RUN_REXT should check to make sure had isn't
	// NULL, so we don't have to cancel/clear any interrupts.
	if(head == v) {
		head = v->next;
		head->prev = NULL;
	}
	if(v->next != NULL) {
		v->next->prev = v->prev;
	}
	if(v->prev != NULL) {
		v->prev->next = v->next;
	}
	v->is_set = false;
	v->ticks = 0;
	v->scheduled_time = 0;
	v->next = NULL;
	v->prev = NULL;

	// If there is a new head, schedule it.
	if(head != NULL && !head->is_scheduled) {
		vtimer_arch_schedule(head->scheduled_time, head->ticks);
	}
	else if(head == NULL) { // we canceled the only vtimer
		vtimer_arch_cancel(); // disable further interrupts from happening
	}
	INTERRUPTS_ENABLE();
}

/**
   Callback from the hardare timer interrupt. User set vtimer callbacks run inside this interrupt context, so they
   should be short and interrupt safe.
*/
void vtimer_run_next() {
	if(head != NULL) {
		vtimer *current = head;
		head = head->next;
		current->is_set = false;
		current->is_scheduled = false;
		current->ticks = 0;
		current->scheduled_time = 0;
		current->prev = NULL;
		current->next = NULL;
		(*(current->callback))();
		if(head != NULL && head->is_scheduled == false) {
			head->is_scheduled = true;
			head->prev = NULL;
			if(head->next != NULL) {head->next->is_scheduled = false;}
			vtimer_arch_schedule(head->scheduled_time, head->ticks);
		}
	}
}


void vtimer_init() {
	head = NULL;
	vtimer_arch_init();
}

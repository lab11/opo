#include "contiki.h"
#include "cloudcomm.h"
#include "opo.h"
#include "lib/random.h"
#include "vtimer.h"
#include "simplestore.h"
#include "dev/gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc2538-rf-debug.h"

PROCESS(plugtest, "opo8001rxtx");
AUTOSTART_PROCESSES(&plugtest);
struct vtimer plug_debouncer;
struct vtimer unplug_debouncer;

static void clear_and_disable_plug_int() {
	GPIO_DISABLE_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_DISABLE_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
}

static void plug_detect(uint8_t port, uint8_t pin) {
	clear_and_disable_plug_int();
	schedule_vtimer(&plug_debouncer, VTIMER_SECOND);
}

static void unplug_detect(uint8_t port, uint8_t pin) {
	clear_and_disable_plug_int();
	schedule_vtimer(&unplug_debouncer, VTIMER_SECOND);
}

static void setup_plug_detect_pin() {
	gpio_register_callback(plug_detect, PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_NUM);
	GPIO_DETECT_RISING(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_POWER_UP_ON_RISING(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_ENABLE_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_ENABLE_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
}

static void setup_unplug_detect_pin() {
	gpio_register_callback(unplug_detect, PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_NUM);
	GPIO_DETECT_FALLING(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_POWER_UP_ON_FALLING(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_ENABLE_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_ENABLE_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
}

static void plug_debouncer_callback() {
	leds_off(LEDS_RED);
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == PLUG_DETECT_PIN_MASK) {
		leds_on(LEDS_BLUE);
		setup_unplug_detect_pin();
	}
	else {
		char buffer[100];
		uint32_t test = GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
		snprintf(buffer, 100, "Read pin %lx\n", test);
		send_rf_debug_msg(buffer);
	}
}

static void unplug_debouncer_callback() {
	leds_off(LEDS_BLUE);
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == 0) {
		leds_on(LEDS_GREEN);
	}
	else {
		leds_on(LEDS_BLUE);
	}
}

PROCESS_THREAD(plugtest, ev, data) {
	PROCESS_BEGIN();

	plug_debouncer = get_vtimer(plug_debouncer_callback);
	unplug_debouncer = get_vtimer(unplug_debouncer_callback);

	GPIO_SOFTWARE_CONTROL(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_SET_INPUT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);

	setup_plug_detect_pin();
	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_A);
	leds_on(LEDS_RED);
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK)  == PLUG_DETECT_PIN_MASK) {
		plug_detect(0,0);
	}
	PROCESS_END();
}

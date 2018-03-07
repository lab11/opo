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

PROCESS(opo8001rxtx, "opo8001rxtx");
PROCESS(opo8001rx, "opo8001rx");
PROCESS(opo8001tx, "opo8001tx");
PROCESS(opo8001ccon, "enable cloudcomm");
PROCESS(plugin_manager, "plugin manager");
AUTOSTART_PROCESSES(&opo8001rxtx);
static struct vtimer tx_delay;
static cloudcomm_meta_t cc_metadata;
static char murl[] = "http://post.gatd.io/a7ffd6e1-a973-4670-a2d3-4e79c9a4a390";
static opo_data_t mdata;
static void cloudcomm_callback();
static bool sending_fail = false;

/* State variables for our plug in detector */
static bool plugged_in = false;
static bool cc_done = false;
static bool plugged_in_on_start = false;
struct vtimer plug_debouncer;
struct vtimer unplug_debouncer;
static vtimer plugin_checker;

struct vtimer blink_red_vtimer;
struct vtimer blink_blue_vtimer;
struct vtimer blink_green_vtimer;

static void blink_red() {
	leds_on(LEDS_RED);
	schedule_vtimer_ms(&blink_red_vtimer, 100);
}

static void blink_blue() {
	leds_on(LEDS_BLUE);
	schedule_vtimer_ms(&blink_blue_vtimer, 100);
}

static void blink_green() {
	leds_on(LEDS_GREEN);
	schedule_vtimer_ms(&blink_green_vtimer, 100);
}

static void blink_red_callback() { leds_off(LEDS_RED); }
static void blink_blue_callback() { leds_off(LEDS_BLUE); }
static void blink_green_callback() { leds_off(LEDS_GREEN); }

static void clear_and_disable_plug_int() {
	GPIO_DISABLE_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_DISABLE_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
}

static void plug_detect(uint8_t port, uint8_t pin) {
	send_rf_debug_msg("Plugged in\n");
	clear_and_disable_plug_int();
	schedule_vtimer_ms(&plug_debouncer, 1000);
}

static void unplug_detect(uint8_t port, uint8_t pin) {
	send_rf_debug_msg("Unplugged\n");
	clear_and_disable_plug_int();
	cancel_vtimer(&plugin_checker);
	schedule_vtimer_ms(&unplug_debouncer, 1000);
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

/* Check to see if we are still plugged int. */
static void plugin_checker_callback() {
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) != PLUG_DETECT_PIN_MASK) {
		send_rf_debug_msg("Not plugged in anymore, somehow missed the interrupt?");
		plugged_in = false;
		process_poll(&plugin_manager);
		setup_plug_detect_pin();
	}
	else {
		send_rf_debug_msg("Plugin checker chugging along");
		char buffer[100];
		snprintf(buffer, 100, "Opo_state: %u", get_opo_state());
		send_rf_debug_msg(buffer);
		if(opo8001rx.needspoll) {send_rf_debug_msg("Opo8001rx needspoll");}
		if(opo8001tx.needspoll) {send_rf_debug_msg("Opo8001tx needspoll");}
		if(opo8001ccon.needspoll) {send_rf_debug_msg("Opo8001ccon needspoll");}
		schedule_vtimer_ms(&plugin_checker, 30000);
	}
}

static void plug_debouncer_callback() {
	/* We are stably plugged in, so set state to plugged in and watch for unplug */
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == PLUG_DETECT_PIN_MASK) {
		send_rf_debug_msg("Still plugged in\n");
		plugged_in = true;
		leds_on(LEDS_BLUE);
		leds_off(LEDS_RED);
		leds_off(LEDS_GREEN);
		if(plugged_in_on_start) {
			plugged_in_on_start = false;
			process_poll(&opo8001ccon);
		}
		setup_unplug_detect_pin();
		schedule_vtimer_ms(&plugin_checker, 30000);

	}
	/* We are not actually plugged in? set state to unplugged, resume Opo activities */
	else {
		send_rf_debug_msg("Plugin debouncer fail\n");
		plugged_in = false;
		if(plugged_in_on_start) {plugged_in_on_start = false;}
		process_poll(&plugin_manager);
		setup_plug_detect_pin();
	}
}

static void unplug_debouncer_callback() {
	/* Unplugged, so resume opo activities */
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == 0) {
		send_rf_debug_msg("Still unplugged in\n");
		/* Set state to not plugged in, resume Opo operation if cloudcomm is off, watch for next plugged in event */
		plugged_in = false;
		leds_off(LEDS_BLUE);
		process_poll(&plugin_manager);
		setup_plug_detect_pin();
	}
	/* Still plugged in */
	else {
		send_rf_debug_msg("Unplug debounce fail\n");
		plugged_in = true;
		setup_plug_detect_pin();
		schedule_vtimer_ms(&plugin_checker, 30000);
	}
}
static unsigned short generate_rand_tx() {
	unsigned short r = random_rand();
	unsigned short timer = 2000 > r ? 2000 % r: r % 2000;
	return timer;
}

void init_callback() {
	process_poll(&opo8001rxtx);
}

void opo_rx_callback(opo_data_t odata) {
	memcpy(&mdata, &odata, sizeof(opo_data_t));
	//char debug[100];
	//snprintf(debug, 100, "Opo Rx Timing: %lu, time: %lu\n", mdata.range_dt, mdata.m_unixtime);
	//send_rf_debug_msg(debug);
	if(mdata.range_dt >= 105) {
		blink_blue();
		//leds_off(LEDS_RED);
		//leds_toggle(LEDS_BLUE);
	}
	else {
		//leds_off(LEDS_BLUE);
		//leds_toggle(LEDS_RED);
		blink_red();
	}
	process_poll(&opo8001rx);
}

void tx_delay_callback() {
	//send_rf_debug_msg("Opo Tx Delay Callback");
	process_poll(&opo8001tx);
}

void opo_tx_callback() {
	if(!plugged_in) {
		unsigned short delaytime = generate_rand_tx();
		//char buffer[50];
		//snprintf(buffer, 50, "Opo Tx Go Time: %p\n", &delaytime);
		//send_rf_debug_msg(buffer);
		//leds_toggle(LEDS_GREEN);
		blink_green();
		schedule_vtimer_ms(&tx_delay, 1000 + delaytime);
		enable_opo_rx();
	}
	else {
		//send_rf_debug_msg("Opo Tx CC Time");
		cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
		process_poll(&opo8001ccon);
	}
}

static void cloudcomm_callback() {
	//send_rf_debug_msg("Cloudcomm Done");
	process_poll(&opo8001ccon);
}

PROCESS_THREAD(plugin_manager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		/* Resume opo activities if cloudcomm is done and we are unplugged */
		if(cc_done && !plugged_in) {
			leds_off(LEDS_RED);
			leds_off(LEDS_BLUE);
			cc_done = false;
			unsigned short delaytime = generate_rand_tx();
			schedule_vtimer_ms(&tx_delay, 1000 + delaytime);
			enable_opo_rx();
		}
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001ccon, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		send_rf_debug_msg("Cloudcomm On");
		if(plugged_in) {
			if(cloudcomm_on(cloudcomm_callback, 15) == 0) {
				cc_done = true;
				leds_off(LEDS_RED);
				process_poll(&plugin_manager);
			}
			else { leds_on(LEDS_RED); }
		}
		else {
			cc_done = true;
			process_poll(&plugin_manager);
		}
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		//send_rf_debug_msg("Opo rx process reenable\n");
		cloudcomm_store(&mdata);
		enable_opo_rx();
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001tx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		//send_rf_debug_msg("Perform Opo TX");
		if(!perform_opo_tx()) {
			//send_rf_debug_msg("Opo Tx failed woo");
			unsigned short delaytime = generate_rand_tx();
			//char buffer[100];
			//snprintf(buffer, 100, "Opo Tx Failed: %u\n", delaytime);
			//send_rf_debug_msg(buffer);
			sending_fail = true;
			schedule_vtimer_ms(&tx_delay, 1000 + delaytime);
		}
		else {
			sending_fail = false;
		}
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	register_opo_rx_callback((opo_rx_callback_t) opo_rx_callback);
	register_opo_tx_callback(opo_tx_callback);
	tx_delay = get_vtimer(tx_delay_callback);
	process_start(&opo8001rx, NULL);
	process_start(&opo8001tx, NULL);
	process_start(&opo8001ccon, NULL);
	process_start(&plugin_manager, NULL);

	cc_metadata.dest = &murl[0];
	cc_metadata.dest_len = 56;
	cloudcomm_set_packet_length(sizeof(opo_data_t));
	cloudcomm_set_metainfo(&cc_metadata);
	char buffer[50];
	snprintf(buffer, 50, "Opo Data Size: %u", sizeof(opo_data_t));
	send_rf_debug_msg(buffer);
	cloudcomm_init();
	cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
	cloudcomm_on(init_callback, 0);
	leds_on(LEDS_RED);
	leds_on(LEDS_BLUE); 
	leds_on(LEDS_GREEN);
	PROCESS_YIELD_UNTIL(ev==PROCESS_EVENT_POLL); // cloudcomm has obtained the time.
	leds_off(LEDS_RED);
	plug_debouncer = get_vtimer(plug_debouncer_callback);
	plugin_checker = get_vtimer(plugin_checker_callback);
	unplug_debouncer = get_vtimer(unplug_debouncer_callback);
	blink_red_vtimer = get_vtimer(blink_red_callback);
	blink_blue_vtimer = get_vtimer(blink_blue_callback);
	blink_green_vtimer = get_vtimer(blink_green_callback);
	setup_plug_detect_pin();
	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_A);
	/* Handle case where node is plugged in on boot */
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == PLUG_DETECT_PIN_MASK) {
		plugged_in_on_start = true;
		plug_detect(0,0); // LIkely no interrupt, so manually trigger plug detection
	}
	/* Handle case where node is not plugged in on boot */
	else {
		unsigned short randtime = generate_rand_tx();
		schedule_vtimer(&tx_delay, 1000 +  randtime);
		enable_opo_rx();
	}

	PROCESS_END();
}

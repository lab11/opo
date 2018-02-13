#include "contiki.h"
#include "cloudcomm-802154.h"
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
#include "blink.h"
#include "adxl362.h"
#include "rtc.h"
#include "sst25vf.h"

PROCESS(opo8001rxtx, "opo8001rxtx");
PROCESS(opo8001rx, "opo8001rx");
PROCESS(opo8001tx, "opo8001tx");
PROCESS(pluginManager, "enable cloudcomm");
PROCESS(unplugResume, "plugin manager");
PROCESS(accelActivity, "accel activity");
AUTOSTART_PROCESSES(&opo8001rxtx);

static struct vtimer tx_delay;
static opo_data_t mdata;
static opo_data_t accelData;
static void cloudcomm_callback();
static bool pending_opo_tx = false;

/* State variables for our plug in detector */
static bool plugged_in = false;
static bool cc_done = false;
static bool plugged_in_on_start = false;
static struct vtimer plug_debouncer;
static struct vtimer unplug_debouncer;
static struct vtimer plugin_checker;
static struct vtimer cc_delay; 

static void plug_detect(uint8_t port, uint8_t pin);
static void unplug_detect(uint8_t port, uint8_t pin);


static void clear_and_disable_plug_int() {
	GPIO_DISABLE_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
	GPIO_DISABLE_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(PLUG_DETECT_PORT_NUM, PLUG_DETECT_PIN_MASK);
	GPIO_CLEAR_INTERRUPT(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK);
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

/* Plug in stuff */ 

// After we detect a plugin, set up a debounce checker.
// If we are still plugged in after one second, procceed
static void plug_detect(uint8_t port, uint8_t pin) {
	send_rf_debug_msg("Plugged in\n");
	clear_and_disable_plug_int();
	schedule_vtimer_ms(&plug_debouncer, 1000);
}

// Checks if we are stably plugged in. If we are, setup unplug detection and our unplug safety valve, 
// and then call pluginManager to disable opo rx and motion detection + start up cloudcomm. 
// If we are not plugged in, re-enable plug-in detection and call the unplugResume to resume normal operation.
static void plug_debouncer_callback() {
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == PLUG_DETECT_PIN_MASK) {
		send_rf_debug_msg("Still plugged in\n");
		plugged_in = true;
		leds_off(LEDS_RED);
		leds_off(LEDS_GREEN);
		
		setup_unplug_detect_pin();
		schedule_vtimer_ms(&plugin_checker, 30000);
		process_poll(&pluginManager);
	}
	else {
		send_rf_debug_msg("Plugin debouncer fail\n");
		plugged_in = false;
		setup_plug_detect_pin();
		process_poll(&unplugResume);
	}
}

// Safety valve which makes sure we are plugged in and didn't miss a unplug event.
// If we are unplugged, call unplug_detect to initate upnplug procedures.
static void plugin_checker_callback() {
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) != PLUG_DETECT_PIN_MASK) {
		send_rf_debug_msg("Not plugged in anymore, somehow missed the interrupt?");
		unplug_detect(0,0);
	}
	else {
		send_rf_debug_msg("Plugin checker chugging along");
		char buffer[100];
		snprintf(buffer, 80, "Opo_state: %i", get_opo_state());
		send_rf_debug_msg(buffer);
		if(opo8001rx.needspoll) {send_rf_debug_msg("Opo8001rx needspoll");}
		if(opo8001tx.needspoll) {send_rf_debug_msg("Opo8001tx needspoll");}
		if(pluginManager.needspoll) {send_rf_debug_msg("pluginManager needspoll");}
		schedule_vtimer_ms(&plugin_checker, 30000);
	}
}

/* Unplug stuff */ 

static void unplug_detect(uint8_t port, uint8_t pin) {
	send_rf_debug_msg("Unplugged\n");
	clear_and_disable_plug_int();
	cancel_vtimer(&plugin_checker);
	schedule_vtimer_ms(&unplug_debouncer, 1000);
}

// Checks to see if we are indeed unplugged. If we are, set plugged_in to false, setup plug detection, tell 
// cloudcomm to stop, and call unplugResume to return to normal operation.
// If not, keep on chugging.
static void unplug_debouncer_callback() {
	/* Unplugged, so resume handwashing detection */
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == 0) {
		send_rf_debug_msg("Still unplugged in\n");
		/* Set state to not plugged in, resume Opo operation if cloudcomm is off, watch for next plugged in event */
		leds_off(LEDS_BLUE);
		plugged_in = false;
		cloudcomm_off();
		setup_plug_detect_pin();
		process_poll(&unplugResume);
	}
	/* Still plugged in */
	else {
		send_rf_debug_msg("Unplug debounce fail\n");
		plugged_in = true;
		setup_unplug_detect_pin();
		schedule_vtimer_ms(&plugin_checker, 30000);
	}
}

// If we are unplugged and cloudcomm is done it's shit, then the plugin manager resumes everything.
PROCESS_THREAD(unplugResume, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		/* Resume opo activities if cloudcomm is done and we are unplugged */
		if(cc_done && !plugged_in) {
			send_rf_debug_msg("Unplugged: resume operation");
			leds_off(LEDS_RED);
			leds_off(LEDS_BLUE);
			//sst25vf_turn_on();
			//accel_enable_int_two_awake();
			//accel_enable_loop_mode();
			//sst25vf_turn_off();
			enable_opo_rx();
		}
	}
	PROCESS_END();
}
static void cloudcomm_callback() {
	//send_rf_debug_msg("Cloudcomm Done");
	process_poll(&pluginManager);
}

static void cc_delay_callback() {
	process_poll(&pluginManager);
}

// If we are plugged in, disable opo and motion detection and call cloudcomm on. 
// If cloudcomm finishes / we are unplugged, hand over control to unplugResume.
PROCESS_THREAD(pluginManager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		send_rf_debug_msg("Cloudcomm On");
		if(plugged_in) {
			//sst25vf_turn_on();
			//accel_disable_int_two();
			//sst25vf_turn_off();
			// Check to make sure Opo is idle before starting cloudcomm.
			INTERRUPTS_DISABLE();
			if(get_opo_state() == 3 && !pending_opo_tx) {
				disable_opo_rx();
			}
			else {
				schedule_vtimer_ms(&cc_delay, 100);
				INTERRUPTS_ENABLE();
				continue;
			}
			INTERRUPTS_ENABLE();
			cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
			// Call cloudcomm. If it returns 0, it means we are done, so hand over control to the unplugResume.
			if(cloudcomm_on(cloudcomm_callback) == 0) {
				cc_done = true;
				leds_off(LEDS_RED);
				process_poll(&unplugResume);
			}
			else {
				cc_done = false;
				leds_on(LEDS_RED);
			}
		}
		else {
			cc_done = true;
			process_poll(&unplugResume);
		}
	}
	PROCESS_END();
}

/* ACCEL STUFF */ 

void accel_activity_callback() { process_poll(&accelActivity); blink_red(); }

// Get the time of the callback, record it, setup opo tx.
PROCESS_THREAD(accelActivity, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		send_rf_debug_msg("Motion Detected");
		if(!plugged_in) {
			accelData.m_unixtime = rtc_get_unixtime();
			cloudcomm_store(&accelData);
			send_rf_debug_msg("Motin detected");
			sst25vf_turn_on();
			accel_get_status(); // Clear the interrupt
			sst25vf_turn_off();
			process_poll(&opo8001tx);
		}
		else {
			if(!sst25vf_is_on()) {
				sst25vf_turn_on();
				accel_get_status();
				sst25vf_turn_off();
			}
			else {
				accel_get_status();
			}
		}
	}
	PROCESS_END();
}

/* OPO RX STUFF */

// Copy Opo ranging data to global variable so that the process can copy it in.
void opo_rx_callback(opo_data_t odata) {
	memcpy(&mdata, &odata, sizeof(opo_data_t));
	blink_blue();
	process_poll(&opo8001rx);
}

// Store our opo ranging and re-enable opo rx if there is no pending opo tx. 
// If there is a pending Opo tx, let it go first. Similarly, if we are plugged in, don't do anything.
PROCESS_THREAD(opo8001rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		char buffer[100]; 
		snprintf(buffer, 100, "Opo rx: %lu", mdata.range_dt);
		send_rf_debug_msg(buffer);
		cloudcomm_store(&mdata); 
		// If there is a pending opo transmission, let it go before re-enabling opo rx.
		if(!pending_opo_tx && !plugged_in) {
			enable_opo_rx();
		}
	}
	PROCESS_END();
}

/* Opo TX Stuff */ 

/* Used to handle cases where TX doesn't get to go (opo rx ongoing) */ 
void tx_delay_callback() {
	process_poll(&opo8001tx);
}

// When we are done transmitting, re-enable Opo RX if we are not plugged in
void opo_tx_callback() {
	pending_opo_tx = false;
	blink_green();
	if(!plugged_in) {
		enable_opo_rx();
	}
}

// Transmit whenever we get an accelerometer interrupt.
// If the accelerometer interrupt goes off during a trnasmit, set sending fail = true
// and transmit when the reception is complete.
PROCESS_THREAD(opo8001tx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		//send_rf_debug_msg("Perform Opo TX");
		if(!perform_opo_tx()) {
			pending_opo_tx = true;
			schedule_vtimer_ms(&tx_delay, 100);
		}
	}
	PROCESS_END();
}

void init_callback() {
	process_poll(&opo8001rxtx);
}

PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();

	// Setup acclerometer for activity detection
	accelData.rx_id = 327; 
	accelData.tx_id = 327;
	accelData.range_dt = 327;
	sst25vf_turn_on();
	accel_soft_reset();
	accel_set_activity_threshold(32);
	accel_set_inactivity_threshold(150);
	accel_set_inactivity_timer(5);
	accel_setup_int_two_awake(accel_activity_callback);
	sst25vf_turn_off();
	process_start(&accelActivity, NULL);

	// Setup opo shit
	register_opo_rx_callback((opo_rx_callback_t) opo_rx_callback);
	register_opo_tx_callback(opo_tx_callback);
	tx_delay = get_vtimer(tx_delay_callback);
	process_start(&opo8001rx, NULL);
	process_start(&opo8001tx, NULL);
	process_start(&pluginManager, NULL);
	
	// Setup plug detection
	plug_debouncer = get_vtimer(plug_debouncer_callback);
	plugin_checker = get_vtimer(plugin_checker_callback);
	unplug_debouncer = get_vtimer(unplug_debouncer_callback);
	process_start(&unplugResume, NULL);

	// Set up cloudcomm
	cloudcomm_set_packet_length(sizeof(opo_data_t));
	char buffer[50];
	snprintf(buffer, 50, "Opo Data Size: %u", sizeof(opo_data_t));
	send_rf_debug_msg(buffer);
	cloudcomm_init();
	cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
	cc_delay = get_vtimer(cc_delay_callback);

	// Turn on cloudcomm, which will trigger the process event poll below when it is done
	cloudcomm_on(init_callback);
	leds_on(LEDS_RED);

	// Cloudcomm has finished getting the time / uploading it's data.
	PROCESS_YIELD_UNTIL(ev==PROCESS_EVENT_POLL);
	leds_off(LEDS_RED);

	// Turn on accelerometer.
	sst25vf_turn_on();
	accel_enable_loop_mode();
	sst25vf_turn_off();

	// Activate plugin detection
	setup_plug_detect_pin();
	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_A);


	// Check to see if we are already plugged in. If so, chill, if not, then activate the accelerometer and opo rx.
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == PLUG_DETECT_PIN_MASK) {
		plugged_in_on_start = true;
		plug_detect(0,0); // Likely no interrupt, so manually trigger plug detection
	}
	else {
		enable_opo_rx();
		sst25vf_turn_off();
	} 

	PROCESS_END();
}

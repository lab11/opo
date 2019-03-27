#include "contiki.h"
#include "cloudcomm-802154.h"
#include "rssi_opo.h"
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
#include "rtc.h"
#include "simple_network_driver.h"
#include "packetbuf.h"
#include "blink.h"

#define MAX_ID 200

PROCESS(opo8001rxtx, "opo8001rxtx");
PROCESS(rssi_rf_rx_proccess, "opo8001rx");
PROCESS(rssi_rf_rx_received_proccess, "received data");
PROCESS(rssi_rf_rx_done_proccess, "done listening");
PROCESS(rssi_rf_tx_proccess, "opo8001tx");
PROCESS(opo8001ccon, "enable cloudcomm");
PROCESS(plugin_manager, "plugin manager");
AUTOSTART_PROCESSES(&opo8001rxtx);

static cloudcomm_meta_t cc_metadata;
static char murl[] = "http://post.gatd.io/a7ffd6e1-a973-4670-a2d3-4e79c9a4a390";
static void cloudcomm_callback();
static uint32_t rx_base_time = 3500; // base time between rx time in ms
static uint32_t rx_on_time = 100; // stay on for 100 ms 
static uint32_t tx_base_time = 10; // we transmit randomly between 10 and 50 ms;
static bool pending_tx = false;
static bool currently_tx = false;
static bool pending_rx = false;
static bool currently_rx = false;
static opo_data_t mdata;
static opo_rmsg_t sending_data;
static opo_rmsg_t rxmsg_storage;
static struct vtimer rf_rx_vtimer; // vtimer to schedule the next listen period
static struct vtimer rf_rx_on_vtimer; // vtimer to know when to stop listneing. 
static struct vtimer rf_tx_vtimer; // vtimer to schedule transmission. 

static uint8_t rssi_counts[MAX_ID] = {0}; // Keep track of how many times we've seen each sensor for this listen period
static struct opo_storage_unit rssi_tracker[MAX_ID] = {0};
static uint32_t listen_time = 0;

/* State variables for our plug in detector */
static bool plugged_in = false;
static bool cc_done = false;
static bool plugged_in_on_start = false;
struct vtimer plug_debouncer;
struct vtimer unplug_debouncer;
static vtimer plugin_checker;

// got a packet
static void rf_rx_received_callback() {
	process_poll(&rssi_rf_rx_received_proccess);
}


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
		char buffer[100];
		uint32_t my_time = rtc_get_unixtime(); 
		snprintf(buffer, 100, "PLUGIN_TIME: %lu", my_time);
		send_rf_debug_msg(buffer);
		schedule_vtimer_ms(&plugin_checker, 30000);
	}
}

static void plug_debouncer_callback() {
	/* We are stably plugged in, so set state to plugged in and watch for unplug */
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == PLUG_DETECT_PIN_MASK) {
		send_rf_debug_msg("Still plugged in\n");
		plugged_in = true;
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
		cloudcomm_off();
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

// spread out our transmissions
static unsigned short generate_tx_buffer() {
	unsigned short r = random_rand();
	unsigned short timer = 40 > r ? 40 % r: r % 40;
	return timer;
}

// spread out listening times so we dont' collide
static unsigned short generate_rx_buffer() {
	unsigned short r = random_rand();
	unsigned short timer = 3000 > r ? 3000 % r: r % 3000;
	return timer;
}

void init_callback() {
	simple_network_set_callback(rf_rx_received_callback);
	process_poll(&opo8001rxtx);
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
			simple_network_set_callback(rf_rx_received_callback);
			schedule_vtimer_ms(&rf_tx_vtimer, tx_base_time + generate_tx_buffer());
			schedule_vtimer_ms(&rf_rx_vtimer, rx_base_time + generate_rx_buffer());
			//enable_opo_rx();
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
			if(cloudcomm_on(cloudcomm_callback) == 0) {
				cc_done = true;
				leds_off(LEDS_RED);
				process_poll(&plugin_manager);
			}
			else {
				leds_on(LEDS_RED);
			}
		}
		else {
			cc_done = true;
			process_poll(&plugin_manager);
		}
	}
	PROCESS_END();
}

static void rf_rx_vtimer_callback() {
	process_poll(&rssi_rf_rx_proccess); 
}

static void rf_rx_on_vtimer_callback() {
	process_poll(&rssi_rf_rx_done_proccess);
}

// start receiving radio signals
PROCESS_THREAD(rssi_rf_rx_proccess, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(!plugged_in) {
			if(!currently_tx) {
				currently_rx = true;
				pending_rx = false;
				blink_green();
				listen_time = rtc_get_unixtime();
				NETSTACK_MAC.on();
				schedule_vtimer_ms(&rf_rx_on_vtimer, rx_on_time);
			}
			else {
				pending_rx = true;
				currently_rx = false;
			}
		}
	}
	PROCESS_END();
}

// got something!
PROCESS_THREAD(rssi_rf_rx_received_proccess, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(currently_rx) {
			uint16_t packet_length = packetbuf_datalen();
			if(packet_length == sizeof(opo_rmsg_t)) {
				/* We have to copy data out of the packetbuf before checking the preamble */
				uint8_t rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
				packetbuf_copyto((void *) &rxmsg_storage);
				if(rxmsg_storage.preamble == (uint16_t) ~(rxmsg_storage.id)) { // need cast because C or shitty compiler
					uint16_t tx_id = rxmsg_storage.id;
					if(rssi_counts[tx_id] <  8) {
						if(rssi_counts[tx_id] < 4) {
							rssi_tracker[tx_id].range_dt |= (rssi << (8 * rssi_counts[tx_id]));
						}
						else {
							rssi_tracker[tx_id].failed_rx_count |= (rssi < (8 * (7 - rssi_counts[tx_id]))); 
						}
						rssi_tracker[tx_id].tx_unixtime = rxmsg_storage.unixtime;
						rssi_counts[tx_id] += 1;
					}

					sending_data.last_interaction_partner_id = mdata.tx_id;
					sending_data.last_unixtime = listen_time; 
					sending_data.last_range_dt = rssi;
					
					blink_blue();
				}
			}
			packetbuf_clear();
		}
		//send_rf_debug_msg("Opo rx process reenable\n");
		//cloudcomm_store(&mdata);
		//enable_opo_rx();
	}
	PROCESS_END();
}

// Recevie time over
PROCESS_THREAD(rssi_rf_rx_done_proccess, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(currently_rx) {
			currently_rx = false;
			pending_rx = false;
			NETSTACK_MAC.off(0);
			
			send_rf_debug_msg("Time to store our rssis");
			for(uint8_t i=0; i < MAX_ID; i++) {
				if(rssi_counts[i] > 0) {
					rssi_tracker[i].rx_id = get_opo_id(); 
					rssi_tracker[i].m_unixtime = listen_time;
					send_rf_debug_msg("RSSI has shit");
					cloudcomm_store(&rssi_tracker[i]);
				}
				rssi_tracker[i].range_dt = 0;
				rssi_tracker[i].failed_rx_count = 0; 
				rssi_counts[i] = 0;
			}
			listen_time = 0; 
			if(pending_tx) {
				process_poll(&rssi_rf_tx_proccess);
			}
			schedule_vtimer_ms(&rf_rx_vtimer, rx_base_time + generate_rx_buffer());
		}
	}
	PROCESS_END();
}

// when our timer gets triggered, call call rf_tx_proccess
static void rf_tx_timer_callback() {
	process_poll(&rssi_rf_tx_proccess);
}

// if we are not receiving, send. if we are, set pending tx to true
// if we transmit, set up the next transmit timer and check to see if 
// the rxtimer is waiting to go. 
PROCESS_THREAD(rssi_rf_tx_proccess, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		//send_rf_debug_msg("Perform Opo TX");
		if(!plugged_in) {
			if(!currently_rx) {
				pending_tx = false;
				currently_tx = true;
				sending_data.unixtime = rtc_get_unixtime(); 
				packetbuf_clear();
				packetbuf_copyfrom((void *) &sending_data, sizeof(opo_rmsg_t));
				REG(RFCORE_XREG_TXPOWER) = 0x42; // Set up tx power here. From cc2538-rf.c in contiki/cpu/c2538
				cc2538_on_and_transmit(0);
				NETSTACK_MAC.off(0);
				REG(RFCORE_XREG_TXPOWER) = 0xB6; // reset transmit to 0 dB 
				//blink_green();
				pending_tx = false; 
				currently_tx = false;
				schedule_vtimer_ms(&rf_tx_vtimer, tx_base_time + generate_tx_buffer()); // schedule another transmission
				if(pending_rx) {
					process_poll(&rssi_rf_rx_proccess); // deal with any pending reception proccess.
				}
			}
			else { // currently receiving. schedule transmission when done. 
				pending_tx = true;
				currently_tx = false;
			}
		}
		else {
			//send_rf_debug_msg("Opo Tx CC Time");
			cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
			process_poll(&opo8001ccon);
		}
		
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	// set up rf LPL
	rf_tx_vtimer = get_vtimer(rf_tx_timer_callback);
	rf_rx_vtimer = get_vtimer(rf_rx_vtimer_callback); 
	rf_rx_on_vtimer = get_vtimer(rf_rx_on_vtimer_callback);
	sending_data.id = get_opo_id();
	sending_data.preamble =  (uint16_t)(~sending_data.id);
	mdata.rx_id = get_opo_id();
	mdata.version_num = 100; 
	for(uint8_t i = 0; i < MAX_ID; i++) {
		rssi_tracker[i].tx_id = i;
	}

	// set up proccesses 
	process_start(&rssi_rf_rx_proccess, NULL); // starts listening period
	process_start(&rssi_rf_rx_received_proccess, NULL); // handles heard packets
	process_start(&rssi_rf_rx_done_proccess, NULL); // finishes listening period
	process_start(&rssi_rf_tx_proccess, NULL); // handles transmissions
	process_start(&opo8001ccon, NULL);
	process_start(&plugin_manager, NULL);

	simple_network_set_callback(rf_rx_received_callback);

	cc_metadata.dest = &murl[0];
	cc_metadata.dest_len = 56;
	cloudcomm_set_packet_length(sizeof(opo_data_t));
	cloudcomm_set_metainfo(&cc_metadata);
	char buffer[50];
	snprintf(buffer, 50, "Opo Data Size: %u", sizeof(opo_data_t));
	send_rf_debug_msg(buffer);
	cloudcomm_init();
	cloudcomm_request_data(CLOUDCOMM_REQ_TIME);
	cloudcomm_on(init_callback);
	leds_on(LEDS_RED);

	PROCESS_YIELD_UNTIL(ev==PROCESS_EVENT_POLL); // cloudcomm has obtained the time.
	leds_off(LEDS_RED);
	plug_debouncer = get_vtimer(plug_debouncer_callback);
	plugin_checker = get_vtimer(plugin_checker_callback);
	unplug_debouncer = get_vtimer(unplug_debouncer_callback);
	setup_plug_detect_pin();
	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_A);
	/* Handle case where node is plugged in on boot */
	if(GPIO_READ_PIN(PLUG_DETECT_PORT_BASE, PLUG_DETECT_PIN_MASK) == PLUG_DETECT_PIN_MASK) {
		plugged_in_on_start = true;
		plug_detect(0,0); // LIkely no interrupt, so manually trigger plug detection
	}
	/* Handle case where node is not plugged in on boot */
	else {
		schedule_vtimer_ms(&rf_tx_vtimer, tx_base_time + generate_tx_buffer());
		schedule_vtimer_ms(&rf_rx_vtimer, rx_base_time + generate_rx_buffer());
	}

	PROCESS_END();
}

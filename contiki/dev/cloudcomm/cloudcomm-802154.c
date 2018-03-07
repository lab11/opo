/*
	Very Opo specific cloudcomm library using the 802.15.4 radio. Hacked together since I had a bunch of Opos made where the BLE won't work.
*/

#include "cloudcomm-802154.h"
#include "cc2538-rf-debug.h"
#include "rtc.h"
#include "simple_network_driver.h"
#include "dev/cc2538-rf.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "lib/random.h"

PROCESS(cloudcomm_manager, "CCData");


// Destination hostname
static cloudcomm_meta_t metainfo;

// What things we want updated (time, location, stepcount, etc)
static uint8_t req_count = 0;
static bool    req_queue[CLOUDCOMM_REQ_QUEUE_LENGTH] = {0};

// Data storage shit
static uint16_t  cc_packet_length = 0;
static uint8_t   data_store[270] = {0};
static uint16_t  data_store_index = 0;
static uint32_t  flash_pages_stored = 0;

// Stuff for sending data;

static const uint8_t max_rf_data_length = 88;  // 4 opo packets
static uint8_t  sequence_num = 1;                // 1 starts a new packet to be sent to the cloud. 255 ends the packet.
static uint8_t  last_sequence_num = 0;           // Used for data ACKS from the phone
static uint8_t  sending_data_store[270] = {0};   // Buffer where data is stored while being sent to the phone
static uint16_t sending_data_store_index = 0;    // How much data is in our sending buffer
static uint16_t next_packet_index = 0;           // Demarcates next data packet. Only used for debug?
static uint16_t last_packet_index = 0;           // Rollback index to handle unexpected disconnect
static uint16_t sending_data_store_end = 0;      // Index of end of valid data in our data buffer
static uint16_t max_data_store_length = 0;       // Bytes of valid data per flash page
static bool     sending_data_store_empty = true;
static bool     sent_data_from_flash = false;    // Checks if the data sent was from flash.
static bool 	rf_packet_acked = true;         // If rf packet is acked, set the time or send next piece of data.
static bool 	time_req = false;
static uint8_t  packet_data_store[200] = {0};	 // Store our last sent packet in case we need it for retransmission
static uint8_t  packet_data_store_len = 0;
static uint8_t  last_rx_id[2] = {0}; // THe rx_id in the opo data packet we transmitted to the gateway

//Cloudcomm operation/shutdown parameters
static void     (*cc_done_callback)();
static vtimer    cc_rf_retransmit_vtimer; // continuously retransmits the data/time request packet until we get an ack

//Cloudcomm State
static bool cc_on = false;
static bool user_control_returned = true;

static cloudcomm_callback_t callbacks[CLOUDCOMM_REQ_QUEUE_LENGTH];

static void default_callback(uint8_t packet[30], uint8_t len) {return;}
static void default_cc_callback() {return;}

/* Cleanup function for cloudcomm. Resets all relevent state variables for next cc connection. */
static void reset_state() {
	sequence_num = 1;
	last_sequence_num = 0;
	cc_on = false;
	rf_packet_acked = true;
	NETSTACK_MAC.off(0);
	simplestore_turn_off_flash();
}

static bool have_tasks() {
	if(req_count > 0 || data_store_index > 0 || flash_pages_stored > 0) {return true;}
	return false;
}

static void return_control_to_user() {
	if(!user_control_returned) {
		user_control_returned = true;
		char buffer[100];
		snprintf(buffer, 100, "CC USER CONTROL: flash_pages_store: %lu", flash_pages_stored);
		send_rf_debug_msg(buffer);
		void (*temp)() = cc_done_callback;
		cc_done_callback = default_cc_callback;
		reset_state();
		(*temp)();
	}
}

/*************************** 802154 CALLBACKS ********************************/

static void cc_rf_retransmit_vtimer_callback() {
	send_rf_debug_msg("CC RF RETRANSMIT TIMER");
	process_poll(&cloudcomm_manager);
}

/* This is an interrupt handler in name only. This actually occurs in a proccess context, so it's interrupt safe. */ 
static void cc_rf_rx_handler() {
	uint8_t data_length = packetbuf_datalen();
    uint8_t *data_ptr = packetbuf_dataptr();
	if(time_req) { // if we requested the time, check to see if this is a time packet and set time
		if(data_length == 8) {
			if(data_ptr[0] == TRIUMVI_RTC && data_ptr[1] == TRIUMVI_RTC_SET) {
				time_req = false;
				req_count = 0;
				req_queue[CLOUDCOMM_REQ_TIME] = false;
				uint8_t time_buf[8];
				time_buf[0] = 0;
				time_buf[1] = data_ptr[7];
				time_buf[2] = data_ptr[6];
				time_buf[3] = data_ptr[5];
				time_buf[4] = 0;
				time_buf[5] = data_ptr[4];
				time_buf[6] = data_ptr[3];
				time_buf[7] = data_ptr[2];
				rv4162_set_time(time_buf);
			}
		}
	}
	else { // check to see if this is an ack for our data packet
		if(data_length == 14) {
			leds_on(LEDS_BLUE);
			if( data_ptr[2] == last_rx_id [0] && data_ptr[3] == last_rx_id[1] \
				&& data_ptr[0] == (uint8_t) ~data_ptr[2] && data_ptr[1] == (uint8_t) ~data_ptr[3] \
				&& data_ptr[4] == last_sequence_num ) {
				leds_on(LEDS_GREEN);
				rf_packet_acked = true;
			}
		}
	}
	process_poll(&cloudcomm_manager);
}

/*************************** END 802154 CALLBACKS ****************************/

/*************************** CLOUDCOMM PROCESSES ******************************/

/*
  * Load data packets into packetbuf and send it out
  * CURRENT_DATA_INDEX: current read index for our data buffer. we update this while uploading data.
  * FINAL_DATA_INDEX: Last index of valid data in data_buffer. In case of flash, always a full page, in case of RAM it is variable.
  * DATA_BUFFER: Contains 1 or more packets of data ready for upload
*/
static void send_rf_packet(uint16_t *current_data_index, uint16_t final_data_index, uint8_t *data_buffer) {
	uint8_t i = 0;
	char buffer[100];
	snprintf(buffer, 100, "CC_SEND: CURRENT_DATA_INDEX: %u", *current_data_index);
	send_rf_debug_msg(buffer);

	uint8_t rf_data_size = max_rf_data_length / cc_packet_length * cc_packet_length; 
	uint16_t remaining_data = (1 + final_data_index) - *current_data_index; // gotta add the +1 to account for the fact that current data index represents the first UNSENT piece of data.
	if(remaining_data < max_rf_data_length) {
		rf_data_size = remaining_data;
	}

	if(rf_data_size % cc_packet_length != 0) { send_rf_debug_msg("RF_PACKET_SIZE ERROR"); }
	
	uint8_t rf_packet_size = 1 + 2 + rf_data_size + 4; // sequence, preamble, 4 extra for opo rf function.
	uint8_t rf_buf[rf_packet_size]; 
	rf_buf[0] = sequence_num;
	for(i=0;i<rf_data_size;i++) {rf_buf[i+3] = sending_data_store[*current_data_index + i]; } // Load URL into ble packet
	rf_buf[1] = ~rf_buf[5]; // put in the preamble
	rf_buf[2] = ~rf_buf[6];

	sequence_num += 1;
	last_sequence_num = rf_buf[0]; // keep track of sequence num for acks.
	last_rx_id[0] = rf_buf[5]; // Keep track of the rx_id in the opo packet for acks
	last_rx_id[1] = rf_buf[6];
	*current_data_index += (rf_data_size); // update current_data_index to the next UNREAD opo packet
	packetbuf_clear();
	packetbuf_copyfrom((void *) rf_buf, rf_packet_size);
	packet_data_store_len = rf_packet_size;
	for(i = 0; i < packet_data_store_len; i++) {
		packet_data_store[i] = rf_buf[i];
	}
	cc2538_on_and_transmit(0);
}

/* Wrapper function for send_ble_apcket() used to make sure current_data_index doesn't exceed sending_data_store_end */
// For now I've taken away this check and just moved cc_rf_rx_handler to check >= sending_data_store_end. 
// My reasoning is that the interrupt of rf ack could happen before send_rf_packet returns.
static void send_cc_data_packet(uint16_t *current_data_index, uint16_t final_data_index, uint8_t *data_buffer) {
	send_rf_packet(current_data_index, final_data_index, data_buffer);
	//if(*current_data_index > sending_data_store_end) {*current_data_index = sending_data_store_end;}
}

/* */
static void load_rfbuf_from_ram() {
	sending_data_store_end = data_store_index - 1; // data_store_index is the first empty index in our ram buffer
	sending_data_store_empty = false;
	sending_data_store_index = 0; // reset SENDING_DATA_STORE read index

	sequence_num = 1;
	last_packet_index = 0;
	next_packet_index = cc_packet_length;

	memcpy(sending_data_store, data_store, data_store_index); // 
	data_store_index = 0;
}

/* use data from flash for cloud upload. We do this in order from oldest to newest. */
static void load_rfbuf_from_flash() {
	sending_data_store_index = 0;

	sequence_num = 1;
	last_packet_index = 0;
	next_packet_index = cc_packet_length;

	uint8_t status = simplestore_read_next_page(sending_data_store, max_data_store_length);
	if(status == SIMPLESTORE_SUCCESS) {
		send_rf_debug_msg("CC_UPLOAD_DATA: load from flash success\n");
		sending_data_store_end = max_data_store_length-1;
		sending_data_store_empty = false;
		if(flash_pages_stored == 0) {
			send_rf_debug_msg("CC_UPLOAD_DATA: Error with flash_pages_stored");
		}else {
			flash_pages_stored -= 1;
		}
	}
	else if (status == SIMPLESTORE_READ_FULL) {
		send_rf_debug_msg("CC_UPLOAD_DATA: nothing left in flash\n");
		sending_data_store_end = 0;
		sending_data_store_empty = true;
		flash_pages_stored = 0;
		simplestore_clear_flash_chip();
	}
	else {
		send_rf_debug_msg("CC_UPLOAD_DATA: flash error\n");
		sending_data_store_end = 0;
		sending_data_store_empty = true;
		flash_pages_stored = 0;
	}
}

/* Upload our sensor data to the cloud */
static void upload_data() {
	/* If our BLE buffer is empty, try and fill it */
	if(sending_data_store_empty) {
		if(data_store_index > 0) {
			send_rf_debug_msg("CC_UPLOAD_DATA: load from ram\n");
			sent_data_from_flash = false;
			load_rfbuf_from_ram();
		}
		else if(flash_pages_stored > 0) {
			send_rf_debug_msg("CC_UPLOAD_DATA: load from flash\n");
			if(sent_data_from_flash) {
				send_rf_debug_msg("CC_MANAGER: Mark last page done");
				simplestore_mark_last_page_done();
			}
			sent_data_from_flash = true;
			load_rfbuf_from_flash();
		}
	}

	/* If our rf buffer has data, send it out */
	if(!sending_data_store_empty) {
		char buffer[100];
		snprintf(buffer, 100, "CC_UPLOAD_DATA: upload data %u %u", sending_data_store_index, sending_data_store_end);
		send_rf_debug_msg(buffer);
		send_cc_data_packet(&sending_data_store_index, sending_data_store_end, sending_data_store);
	}
	/* No data left, Cloudcomm is done */
	else {
		return_control_to_user();
	}

}

static unsigned short generate_retransmit_rand() {
	unsigned short r = random_rand();
	unsigned short timer = 50 > r ? 50 % r: r % 50;
	return timer;
}

/*
  * Manages data upload and metadata aquisition between node and smartphone.
  *
*/
PROCESS_THREAD(cloudcomm_manager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		cancel_vtimer(&cc_rf_retransmit_vtimer);
		if(cc_on) {
			/* Get the time */
			if(req_count > 0) {
				send_rf_debug_msg("CC_MAN: Get time");
				// Time buf contains TRIUMVI_RTC and TRIUMVI_RTC_REQ from edisonOpo.c Also need a 4 byte
				// buffer at the end since we use Opo's rf
				// functions, which append the rf timediff between send and SFD to each packet
				uint8_t time_buf[6] = {172, 254, 0, 0, 0, 0}; // 0xac 0xfe
				packetbuf_clear();
				packetbuf_copyfrom(time_buf, 6);
				time_req = true;
				cc2538_on_and_transmit(0);
				schedule_vtimer_ms(&cc_rf_retransmit_vtimer, 300 + generate_retransmit_rand());
			}
			/* Upload data if there is data in our cc rf storage, cc ram storage, or in flash */
			else if(data_store_index > 0 || flash_pages_stored > 0 || !sending_data_store_empty) {
				simplestore_turn_on_flash(); // Make sure flash is manually turned on.
				if(rf_packet_acked) { // Last piece of data acked by gateway, so send new data
					leds_off(LEDS_BLUE);
					leds_off(LEDS_GREEN);
					send_rf_debug_msg("CC_MAN: Send new RF data");
					rf_packet_acked = false;
					// Check to see if anything is left in the sending data store
					if(sending_data_store_index >= sending_data_store_end) { 
						sending_data_store_empty = true;
					}
					upload_data();
				}
				else{ // Last piece of data not acked by gateway.
					send_rf_debug_msg("Retransmit previous data");
					packetbuf_clear();
					packetbuf_copyfrom(packet_data_store, packet_data_store_len);
					cc2538_on_and_transmit(0);
				}
				schedule_vtimer_ms(&cc_rf_retransmit_vtimer, 300 + generate_retransmit_rand());
			}
			/* Done all data upload/download */
			else {
				send_rf_debug_msg("CC_MAN: Nothing to do");
				return_control_to_user();
			}
		}
		else { // if cloudcomm has been turned off, give shit back to the user.
			send_rf_debug_msg("CC_MAN: CC is off");
			return_control_to_user();
		}
	}
	PROCESS_END();
}

/*************************** END CLOUDCOMM PROCESSES **************************/

void cloudcomm_init() {
	uint8_t i = 0;
	for(i=0;i<CLOUDCOMM_REQ_QUEUE_LENGTH;i++) {
		callbacks[i] = (cloudcomm_callback_t) default_callback;
	}

	simplestore_config();
	flash_pages_stored = simplestore_pages_stored();
	char buffer[100];
	snprintf(buffer, 100, "Cloudcomm init pages stored: %lu", flash_pages_stored);
	send_rf_debug_msg(buffer);
	cc_rf_retransmit_vtimer = get_vtimer(cc_rf_retransmit_vtimer_callback);
	cc_done_callback = default_cc_callback;

	process_start(&cloudcomm_manager, NULL);
}

/*
  * Tell cloudcomm to look for an edison gateway. ontime doesn't do anything in this version, just maintined for compability.
*/
uint8_t cloudcomm_on(void *callback) {
	cc_done_callback = callback;
	user_control_returned = false;
	cc_on = true;
	/* Make sure we have data to upload or metadata to aquire, then connect */
	if(have_tasks()) {
		send_rf_debug_msg("CLOUDCOMM_ON(): connect\n");
		simple_network_set_callback(cc_rf_rx_handler);
		process_poll(&cloudcomm_manager);
		return 1; // CC has shit to do
	}
	else {
		cc_on = false;
		send_rf_debug_msg("CLOUDCOMM_ON(): return control\n");
		user_control_returned = true;
		return 0; // CC HAS NOTHING TO DO
	}
}

void cloudcomm_off() {
	cc_on = false;
}

void cloudcomm_set_metainfo(cloudcomm_meta_t *new_metainfo) {
	metainfo.dest = new_metainfo->dest;
	metainfo.dest_len = new_metainfo->dest_len;
}

void cloudcomm_set_packet_length(uint8_t len) {
	cc_packet_length = len;
	max_data_store_length = len * ( (CLOUDCOMM_PAGE_SIZE - 1) / len);
}

/*
 * Users pass data to Cloudcomm through this function. Cloudcomm buffers this
 * data in DATA_STORE. When the buffer fills up based on CC_PACKET_LENGTH,
 * Cloducomm flushes the data to flash.
*/
uint8_t cloudcomm_store(void *data) {
	uint8_t *data_ptr = (uint8_t *)data;

	/* Add the user provided data to our RAM buffer */
	uint8_t i = 0;
	for(i=0;i < cc_packet_length;i++) { data_store[data_store_index+i] = data_ptr[i]; }

	/* Update data_store_index to point to the next free byte in our RAM buffer */
	data_store_index += cc_packet_length;

	/* If the RAM buffer is full, write it to flash. Leave the last byte in the flash page free as a status byte. */
	/* We consider the buffer full when adding another data packet to it would overflow a flash page (minus last byte for metadata. */
	if( ((uint32_t)data_store_index + (uint32_t)cc_packet_length) > max_data_store_length) {
		uint16_t data_size = data_store_index;
		data_store_index = 0; // Reset our ram buffer
		if(simplestore_write_next_page(data_store, data_size) != SIMPLESTORE_SUCCESS) {
			send_rf_debug_msg("Cloudcomm Store Fail");
			return 0; // something went wrong
		}
		else { 
			send_rf_debug_msg("CC Store success");
			flash_pages_stored += 1; 
		} // update cloudcomm's knowledge of how much data is in flash
	}

	return 1; // everything went smoothly
}

uint8_t cloudcomm_request_data(uint8_t req) {
	if(req < CLOUDCOMM_REQ_QUEUE_LENGTH && req_queue[req] == false) {
		send_rf_debug_msg("CC: Request time");
		req_queue[req] = true;
		req_count += 1;
		return 1;
	}
	else {
		//send_rf_debug_msg("Request cloudcomm data fail");
		return 0;
	}
}

void cloudcomm_clear_data() {
	simplestore_clear_flash_chip();
	flash_pages_stored = 0;
}

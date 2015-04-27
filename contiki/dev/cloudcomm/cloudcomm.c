#include "cloudcomm.h"

PROCESS(ble_connect_process, "BleConnect");
PROCESS(ble_sleep_process, "BleSleep");
PROCESS(open_ccready_rx_pipe, "CCReadyPipe");
PROCESS(open_cc_incoming_data_rx_pipe, "CCIncPipe");
PROCESS(cloudcomm_manager, "CCData");

// Destination hostname
static cloudcomm_meta_t metainfo;

// What things we want updated (time, location, stepcount, etc)
static uint8_t req_count = 0;
static bool    req_queue[CLOUDCOMM_REQ_QUEUE_LENGTH];

// Data storage shit
static uint8_t  cc_packet_length = 0;
static uint8_t  data_store[256] = {0};
static uint8_t  data_store_index = 0;
static uint32_t flash_pages_stored = 0;

// Stuff for sending data;

static uint8_t sequence_num = 1; // 1 starts a new packet to be sent to the cloud. 255 ends the packet.
static uint8_t last_sequence_num = 0;
static uint8_t sending_packet[20] = {0}; // packet we use to send shit to the phone
static uint8_t sending_data_store[256] = {0};
static uint8_t sending_data_store_index = 0; // how much data is in our sending buffer
static uint8_t next_packet_index = 0; // demarcates next data packet
static uint8_t sending_data_store_end = 0;
static uint8_t max_data_store_length = 0;
static bool    sending_data_store_empty = true;

// Stuff for sending over the url
static bool    urlSet = false;
static uint8_t urlIndex = 0;

//Cloudcomm operation/shutdown parameters
static void     (*cc_done_callback)();
static vtimer    cc_vtimer;

//Cloudcomm ble state
static bool on = false; // should we be scanning on ble
static bool sleep = false;
static bool connecting = false; // currently attempting to connect
static bool connected = false; // connected to phone
static bool device_setup_done = false; // nrf8001 setup complete
static bool sending = false; // currently sending data on ble_chip
static bool ble_credited = true; // need both this and data_acked to keep sending
static bool ble_acked = true; // need both this and data_credited to keep sending
static bool phone_ready = false; // Phone has to first tell us it's ready and willing
static bool tried_open_ready = false; // Signifies that we have already tried to open the CCReady pipe
static bool tried_open_data = false; // Signifies that we have already tried to open the CCIncomingData pipe
static bool open_pipes[NUMBER_OF_PIPES];

//Cloudcomm ready pipe byte and bit positions
static uint8_t ccra_byte_pos = 0;
static uint8_t ccra_bit_pos = 0;
//static uint8_t ccr_byte_pos = 0;
//static uint8_t ccr_bit_pos = 0;

//Cloudcomm incoming data pipe byte and bit positions
static uint8_t ccida_byte_pos = 0;
static uint8_t ccida_bit_pos = 0;
//static uint8_t ccid_byte_pos = 0;
//static uint8_t ccid_bit_pos = 0;

static cloudcomm_callback_t callbacks[CLOUDCOMM_REQ_QUEUE_LENGTH];

static void default_callback(uint8_t packet[30], uint8_t len) {}
static void default_cc_callback() {};

static uint8_t get_byte_pos(uint8_t pos) {
	// get byte pos for a pipe in the pipe status array
	uint8_t i = 0;
	uint8_t filter = 0;
	for(i = 0; i < 8; i++) {
		filter = ((i+1) * 8) - 1;
		if (pos < filter) {
			return i;
		}
	}
	return 0;
}

static uint8_t get_bit_pos(uint8_t pos) {
	// get bit position for a pipe in the pipe status array
	return pos % 8;
}



/*************************** NRF8001 CALLBACKS ********************************/

static void sleep_callback() {}

static void device_started_callback(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(payload_length > 0) {
		if(payload[0] == 0x02) {
			nrf8001_setup();
		}
		else if(payload[0] == 0x03) {
			device_setup_done = true;
			if(on) {
				process_poll(&ble_connect_process);
			} else {
				sleep = true;
				process_poll(&ble_sleep_process);
			}
		}
	}
}
/**
   Indicates that we have connected to the phone. However, we can't really do
	 anything until the nrf8001 tells us it's pipes are open, so PIPE_STATUS_HANDLER
	 is functionally our connected handler.
*/
static void connected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	connected = true;
	connecting = false;
}

static void disconnected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	connected = false;
	connecting = false;
	ble_credited = true;
	ble_acked = true;
	phone_ready = false;
	tried_open_ready = false;
	tried_open_data = false;
	urlSet = false;
	cc_done_callback = default_cc_callback;
	if(on && (!sending_data_store_empty || req_count > 0)) {
		process_poll(&ble_connect_process);
	} else {
		sleep = true;
		process_poll(&ble_sleep_process);
	}
}

/**
   Updates us as to what "pipes", ie data channels, are open for use.
	 This is functionally our ble connection established callback.
	 We first make sure that the our RX pipes are available. TX pipe availability
	 is determined by the phone. Then we call CLOUDCOMM_MANAGER, which handles
	 communication with the phone.
*/
static void pipe_status_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected && on) {
		if(payload[ccra_byte_pos] & ccra_bit_pos) {}

		if(!(payload[ccra_byte_pos] & ccra_bit_pos) && tried_open_ready) {
			tried_open_ready = true;
			process_poll(&open_ccready_rx_pipe);
		}
		else if(!(payload[ccida_byte_pos] & ccida_bit_pos) && tried_open_data) {
			tried_open_data = true;
			process_poll(&open_cc_incoming_data_rx_pipe);
		}
		else {
			process_poll(&cloudcomm_manager);
		}
	}
}

/**
   The nrf8001 operates using data_credits. We get them when we first connect
	 to the phone, and when each data transmission completes.
*/
static void data_credit_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected) {
		ble_credited = true;
		sending = false;
		process_poll(&cloudcomm_manager);
	}
}

static void data_ack_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {}

static void data_received_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	uint8_t pipenum = payload[0];
	if(pipenum == PIPE_CLOUDCOMM_CLOUDCOMMACKS_RX_ACK_AUTO) {
		if(payload[1] == sequence_num && connected) {
    		ble_acked = true;
			sending = false;
			process_poll(&cloudcomm_manager);
		}
	}
	else if(pipenum == PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO) {
		uint8_t packet[30];
		uint8_t packet_len = payload_length-2;
		uint8_t i = 0;
		uint8_t service_num = payload[1];
		if(service_num == CLOUDCOMM_REQ_TIME) {
			time_t new_unixtime = 0;
			for(i=0;i<4;i++) {
				new_unixtime <<= 8;
				new_unixtime += payload[1+i];
			}
			set_unixtime(new_unixtime);
		} else {
			for(i=0;i<packet_len;i++) {
				packet[i] = payload[i+2];
			}
			req_count--;
			req_queue[service_num] = false;
			callbacks[service_num](packet, packet_len);
		}
		process_poll(&cloudcomm_manager);
	}
	else if(pipenum == PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO) {
		if(!phone_ready) {
			phone_ready = true;
			process_poll(&cloudcomm_manager);
		}
		else if(payload[1] == last_sequence_num) {
			ble_acked = true;
			sending = false;
			process_poll(&cloudcomm_manager);
		} else {
			if(payload[1] == last_sequence_num - 1) {}
		}
	}
}

static void pipe_error_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
		uint8_t ecode = payload[1];
		if(ecode == ACI_STATUS_ERROR_DATA_SIZE) {}
		else if(ecode == ACI_STATUS_ERROR_PIPE_INVALID) {}
		else if(ecode == ACI_STATUS_ERROR_CREDIT_NOT_AVAILABLE) {}
		else if(ecode == ACI_STATUS_ERROR_PEER_ATT_ERROR) {}
		else if(ecode == ACI_STATUS_ERROR_PIPE_TYPE_INVALID) {}
		else if(ecode == ACI_STATUS_ERROR_PIPE_STATE_INVALID) {}
		else {}
}

/*************************** END NRF8001 CALLBACKS ****************************/

/******************************* NRF8001 PROCESSES ****************************/
PROCESS_THREAD(ble_sleep_process, ev, data) {
	// Setup the BLE chip
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		nrf8001_sleep();
		(*cc_done_callback)();
		cc_done_callback = default_cc_callback;
	}
	PROCESS_END();
}

PROCESS_THREAD(open_ccready_rx_pipe, ev, data) {
	// Setup the BLE chip
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		nrf8001_open_remote_pipe(PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO);
	}
	PROCESS_END();
}

PROCESS_THREAD(open_cc_incoming_data_rx_pipe, ev, data) {
	// Setup the BLE chip
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		nrf8001_open_remote_pipe(PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO);
	}
	PROCESS_END();
}

/**
   Determines if Cloducomm should initiate a BLE connection. Checks to
	 see if user turned cloudcomm on, if we are already connecting, if nrf8001
	 setup is done, and if we have anything to send / any data to request. If not,
	 we put teh chip to sleep
*/
PROCESS_THREAD(ble_connect_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(!connecting && on && (req_count > 0 || data_store_index > 0 || flash_pages_stored > 0) && device_setup_done) {
			// User has turned cloudcomm on, there is data to upload or metadata to collect, and setup is done
			if(sleep) {
				// First make sure BLE chip is awake.
				sleep = false;
				nrf8001_wakeup();
			}
			else {
				// Chip is awake. We check to see if there is data in DATA_STORE, and
				// if so copy it to SENDING_DATA_STORE. If not, check if there is data
				// in flash, and if so, copy some into SENDING_DATA_STORE.
				if(sending_data_store_empty) {
					if(data_store_index > 0) {
						sending_data_store_end = data_store_index - 1; // data_store_index is the first empty index, so set SENDING_DATA_STORE_END to that - 1
						sending_data_store_empty = false;
						sending_data_store_index = 0; // reset SENDING_DATA_STORE read index
						next_packet_index = cc_packet_length;
						memcpy(&sending_data_store, &data_store, 256);
					}
					else if(flash_pages_stored > 0) {
						if(simplestore_read_next_page(&sending_data_store, max_data_store_length) == SIMPLESTORE_SUCCESS) {
							sending_data_store_end = max_data_store_length;
							sending_data_store_empty = false;
							sending_data_store_index = 0;
							next_packet_index = cc_packet_length;
						}
					}
				}
				connecting = true;
				nrf8001_connect(0,32);
			}
		}
		else {
			// User has turned cloudcomm on but there is no data to offload
			// We make sure the BLE chip is asleep, then we call cc_done_callback
			if(!sleep) {
				nrf8001_sleep();
			}
			(*cc_done_callback)();
		}
	}
	PROCESS_END();
}

/*************************** END NRF8001 PROCESSES ****************************/

/*************************** CLOUDCOMM PROCESSES ******************************/

/**
   Helper function for CLOUDCOMM_MANAGER which extracts data fragments from a buffer and fills the BLE fragment with it
   plus a sequence number.

   CURRENT_DATA_INDEX: current read index for our data buffer. we update this while uploading data.
   FINAL_DATA_INDEX: ending index number of the current data packet. DATA_BUFFER can contain multiple packets.
*/
static void send_ble_packet(uint8_t *current_data_index, uint8_t final_data_index, uint8_t *data_buffer) {
	uint8_t i = 0; // for loop helper
	sending = true;
	ble_acked = false; // lets us know when
	ble_credited = false; // lets us know when the nrf8001 can service more transmissions.
	uint8_t remaining_data = final_data_index+1 - *current_data_index; // +1 since we assume data_buffer is zero indexed
	uint8_t payload_length = remaining_data > CC_MAX_DATA_LENGTH ? CC_MAX_DATA_LENGTH : remaining_data; // Calculate data fragment length.
	if(remaining_data == payload_length) { // we're at the end of a data packet.
		sending_packet[0] = 0xFF; // set sequence byte to special ending byte
		sequence_num = 1; // reset sequence_num
	}
	else { // we're within a data packet.
		sending_packet[0] = sequence_num; // update sequence byte
		sequence_num += 1; // update sequence_num.
	}
	for(i=0;i<payload_length;i++) { sending_packet[i+1] = data_buffer[*current_data_index + i]; } // Load URL into ble packet

	last_sequence_num = sending_packet[0]; // keep track of sequence num for acks.
	*current_data_index += payload_length; // update current_data_index to the next UNREAD index in the data_buffer

	nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK, payload_length + 1, &sending_packet[0]);
}

/**
   Manages data offloading and other communcation to the phone.
	 We first send over the URL we would like to upload data to and any
	 other metainfo regarding our node to the phone.
	 Next we service requests for metadata, such as the current time.
	 Finally we offload whatever data we have. The phone then attempts to upload
	 this data to the cloud.

	 Data packets are fragmented into <20 byte blobs, with the first byte
	 in each BLE packet reserved for state. If the first byte is 0xFF, the
	 phone defragments all currently held fragments and uploads it to the cloud.

*/
PROCESS_THREAD(cloudcomm_manager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(!sending && phone_ready && ble_credited && ble_acked && connected) { // first send URL to phone
			if(!urlSet && (req_count > 0 || !sending_data_store_empty)) {
				send_ble_packet(&urlIndex, metainfo.dest_len-1, (uint8_t *)metainfo.dest); // fill BLE packet with data fragment
				if(sending_packet[0] == 0xFF) {urlSet = true;}
			}
			else if(req_count > 0) { // Deal with metainfo like time next.
				uint8_t i;
				for(i=0;i<CLOUDCOMM_REQ_QUEUE_LENGTH;i++) {
					if(req_queue[i]) { // REQ_QUEUE keeps track of what metainfo we want.
						nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX_ACK,1,&i); // send metainfo requests to phone
						break;
					}
				}
			}
			else if (!sending_data_store_empty) { // Offload data to phone
				// Check for unuploaded data in SENDING_DATA_STORE. If SENDING_DATA_STORE_INDEX has exceeded the last
				// index with valid data, check to see if there is any more data in flash. Note that
				// SENDING_DATA_STORE_INDEX will exceed SENDING_DATA_STORE_END because SEND_BLE_PACKET updates
				// SENDING_DATA_STORE_INDEX to the next unread index.
				if(sending_data_store_index >= sending_data_store_end) {
					uint8_t ss_state = simplestore_read_next_page(&sending_data_store, max_data_store_length);
					sending_data_store_index = 0;
					sequence_num = 1;
					next_packet_index = 0;

					if(ss_state == SIMPLESTORE_SUCCESS) { // successfully read more data
						sending_data_store_empty = false;
						sending_data_store_end = max_data_store_length;
					}
					else if(ss_state == SIMPLESTORE_READ_FULL) { // We've already read all data fromn flash
						simplestore_clear_flash_chip();
					}

					// There is no more flash data or something went wrong communicating with the flash chip.
					// Either way, disconnect and return control to other applications.s
					if(ss_state == SIMPLESTORE_FAIL || ss_state == SIMPLESTORE_READ_FULL) {
						sending_data_store_empty = true;
						sending_data_store_end = 0;
						on = false;
						nrf8001_disconnect(0x01);
						continue;
					}
				}
				// If there is more data in the buffer, check to see if we are the end of our current data packet
				else if(sending_data_store_index == next_packet_index) {
					next_packet_index += cc_packet_length; // update data packet demarcater
				}

				send_ble_packet(&sending_data_store_index, next_packet_index-1, &sending_data_store[0]);
			}
			else { // Nothing left to do. Time to disconnect and return control to user;
				on = false;
				nrf8001_disconnect(0x01);
			}
		}
	}
	PROCESS_END();
}

/*************************** END CLOUDCOMM PROCESSES **************************/
static void cc_vtimer_callback() {
	on = false;
	if(!connected) {
		process_poll(&ble_sleep_process);
	}
}

void cloudcomm_init() {
	uint8_t i = 0;
	for(i=0;i<CLOUDCOMM_REQ_QUEUE_LENGTH;i++) {
		callbacks[i] = (cloudcomm_callback_t) default_callback;
	}
	for(i=0;i<NUMBER_OF_PIPES;i++) {
		open_pipes[i] = false;
	}

	ccra_byte_pos = get_byte_pos(PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO);
	ccra_bit_pos = get_bit_pos(PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO);

	ccida_byte_pos = get_byte_pos(PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO);
	ccida_bit_pos = get_bit_pos(PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO);

	simplestore_config();
	flash_pages_stored = simplestore_pages_stored();
	cc_vtimer = get_vtimer(cc_vtimer_callback);
	cc_done_callback = default_cc_callback;

	process_start(&ble_connect_process, NULL);
	process_start(&cloudcomm_manager, NULL);
	process_start(&ble_sleep_process, NULL);
	process_start(&open_ccready_rx_pipe, NULL);
	process_start(&open_cc_incoming_data_rx_pipe, NULL);
	nrf8001_register_callback(NRF8001_DEVICE_STARTED_EVENT, (nrf8001_callback_t) device_started_callback);
	nrf8001_register_callback(NRF8001_CONNECTED_EVENT, (nrf8001_callback_t) connected_handler);
	nrf8001_register_callback(NRF8001_DISCONNECTED_EVENT, (nrf8001_callback_t) disconnected_handler);
	nrf8001_register_callback(NRF8001_PIPE_STATUS_EVENT, (nrf8001_callback_t) pipe_status_handler);
	nrf8001_register_callback(NRF8001_PIPE_ERROR_EVENT, (nrf8001_callback_t) pipe_error_handler);
	nrf8001_register_callback(NRF8001_DATA_CREDIT_EVENT, (nrf8001_callback_t) data_credit_handler);
	nrf8001_register_callback(NRF8001_DATA_RECEIVED_EVENT, (nrf8001_callback_t) data_received_handler);
	nrf8001_register_callback(NRF8001_DATA_ACK_EVENT, (nrf8001_callback_t) data_ack_handler);
	nrf8001_register_sleep_callback(sleep_callback);

	nrf8001_enable();
	nrf8001_reset();
}
/**
   Turn on cloudcomm and try to offload data. Data is offloaded to URl set
	 through CLOUDCOMM_SET_METAINFO. Users provide both a callback and a
	 timeout. If no phone has been found before the timeout, Cloudcomm
	 shuts down and returns control to the user.
*/
void cloudcomm_on(void *callback, uint16_t ontime) {
	on = true; // Change state to ON
	cc_done_callback = callback; // Set callback
	schedule_vtimer(&cc_vtimer, VTIMER_SECOND/1000 * ontime); // Set timetout
	process_poll(&ble_connect_process); // Initiate BLE connection
}

void cloudcomm_set_metainfo(cloudcomm_meta_t *new_metainfo) {
	metainfo.dest = new_metainfo->dest;
	metainfo.dest_len = new_metainfo->dest_len;
}

void cloudcomm_set_packet_length(uint8_t len) {
	cc_packet_length = len;
	max_data_store_length = len * (256/len);
}

/**
   Users pass data to Cloudcomm through this function. Cloudcomm buffers this
	 data in DATA_STORE. When the buffer fills up based on CC_PACKET_LENGTH,
	 Cloducomm flushes the data to flash.
*/
uint8_t cloudcomm_store(void *data) {
	uint8_t i = 0; // for loop helper value
	uint8_t *data_ptr = (uint8_t *)data; // UINT8_T ptr to user data

	for(i=0;i < cc_packet_length;i++) {
		data_store[data_store_index+i] = data_ptr[i]; // Copy data into DATA_STORE buffer
	}
	data_store_index += cc_packet_length; // Update DATA_STORE write index

	// Check to see if DATA_STORE can hold another packet. If not, flush to flash.
	if(data_store_index + cc_packet_length < 255) {
		if(simplestore_write_next_page((void *)&data_store, data_store_index) != SIMPLESTORE_SUCCESS) {
			return 0; // Flash chip full.
		}
		flash_pages_stored += 1; // Update cloudcomm's knowledge of how much data is in flash
		data_store_index = 0; // Reset DATA_STORE
	}

	return 1; // everything went smoothly
}

uint8_t request_cloudcomm_data(uint8_t req) {
	uint8_t result = 1;
	if(req < CLOUDCOMM_REQ_QUEUE_LENGTH) {
		req_queue[req] = true;
		req_count++;
	} else {
		result = 0;
	}
	process_poll(&cloudcomm_manager);
	return result;
}

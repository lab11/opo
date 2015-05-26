#include "cloudcomm.h"
#include "cc2538-rf-debug.h"
#include "rtc.h"
PROCESS(ble_connect_process, "BleConnect");
PROCESS(ble_sleep_process, "BleSleep");
PROCESS(open_ccready_rx_pipe, "CCReadyPipe");
PROCESS(open_cc_incoming_data_rx_pipe, "CCIncPipe");
PROCESS(cloudcomm_manager, "CCData");

// Destination hostname
static cloudcomm_meta_t metainfo;

// What things we want updated (time, location, stepcount, etc)
static uint8_t req_count = 0;
static bool    req_queue[CLOUDCOMM_REQ_QUEUE_LENGTH] = {0};

// Data storage shit
static uint8_t  cc_packet_length = 0;
static uint8_t  data_store[300] = {0};
static uint8_t  data_store_index = 0;
static uint32_t flash_pages_stored = 0;

// Stuff for sending data;

static uint8_t sequence_num = 1; // 1 starts a new packet to be sent to the cloud. 255 ends the packet.
static uint8_t last_sequence_num = 0;
static uint8_t sending_packet[20] = {0}; // packet we use to send shit to the phone
static uint8_t sending_data_store[300] = {0};
static uint8_t sending_data_store_index = 0; // how much data is in our sending buffer
static uint8_t next_packet_index = 0; // demarcates next data packet
static uint8_t last_packet_index = 0; // keep track of rollback index in case of disconnect
static uint8_t sending_data_store_end = 0;
static uint8_t max_data_store_length = 0;
static bool    sending_data_store_empty = true;

// Stuff for sending over the url
static bool    urlSet = false;
static uint8_t urlIndex = 0;

//Cloudcomm operation/shutdown parameters
static void     (*cc_done_callback)();
static vtimer    cc_timeout_vtimer;
static vtimer    cc_failsafe_vtimer;
static uint16_t  cc_ontime = 0;

//Cloudcomm ble state
static bool on = false; // should we be scanning on ble
static bool sleep = false;
static bool connecting = false; // currently attempting to connect
static bool connected = false; // connected to phone
static bool pipes_ready = false; // NRf8001 ready to do shit after connection
static bool device_setup_done = false; // nrf8001 setup complete
static bool sending = false; // currently sending data on ble_chip
static bool ble_credited = true; // need both this and data_acked to keep sending
static bool ble_acked = true; // need both this and data_credited to keep sending
static bool phone_ready = false; // Phone has to first tell us it's ready and willing
static bool tried_open_ready = false; // Signifies that we have already tried to open the CCReady pipe
static bool tried_open_data = false; // Signifies that we have already tried to open the CCIncomingData pipe
static bool open_pipes[NUMBER_OF_PIPES];
static bool failsafe = false; // Used to indicate that the phone connection has gone awry
static bool reset = false;
static bool device_started = false;

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

static void default_callback(uint8_t packet[30], uint8_t len) {return;}
static void default_cc_callback() {return;}

static uint8_t get_byte_pos(uint8_t pos) {
	// get byte pos for a pipe in the pipe status array
	uint8_t i = 0;
	uint8_t filter = 0;
	for(i = 0; i < 8; i++) {
		filter = ((i+1) * 8) - 1;
		if (pos < filter) { return i; }
	}
	return 0;
}

static uint8_t get_bit_pos(uint8_t pos) {
	// get bit position for a pipe in the pipe status array
	return pos % 8;
}

static void reset_state() {
	connected = false;
	connecting = false;
	pipes_ready = false;
	ble_credited = true;
	ble_acked = true;
	phone_ready = false;
	tried_open_ready = false;
	tried_open_data = false;
	urlSet = false;
	urlIndex = 0;
	failsafe = false;
	on = false;
	sending = false;
	device_started = false;

	if(sequence_num != 1) { sending_data_store_index = last_packet_index; }
	sequence_num = 1;
	last_sequence_num = 0;
}

/*************************** NRF8001 CALLBACKS ********************************/

static void sleep_callback() {
	//process_poll(&ble_sleep_process);
	return;
}

static void device_started_callback(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(payload_length > 0) {
		if(payload[0] == 0x02) {
			send_rf_debug_msg("CC Device Setup");
			nrf8001_setup();
		}
		else if(payload[0] == 0x03) {
			device_setup_done = true;
			if(on) {
				if(payload[1] == 0x00) {
					send_rf_debug_msg("CC Device Started");
					device_started = true;
					process_poll(&ble_connect_process);
				}
				else {send_rf_debug_msg ("CC Device Start Fucked");}
			}
			else {
				send_rf_debug_msg("CC Device Start Sleep");
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
	send_rf_debug_msg("CC Con Handl");
}

static void disconnected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	INTERRUPTS_DISABLE();
	reset_state();
	if(cc_ontime == 0 && (!sending_data_store_empty || req_count > 0)) {
		send_rf_debug_msg("CC Discon cont");
		on = true;
		process_poll(&ble_connect_process);
	} else {
		send_rf_debug_msg("CC Discon Sleep");
		on = false;
		process_poll(&ble_sleep_process);
	}
	INTERRUPTS_ENABLE();
}

/**
	Updates us as to what "pipes", ie data channels, are open for use.
	This is functionally our ble connection established callback.
	We first make sure that the our RX pipes are available. TX pipe availability
	is determined by the phone. Then we call CLOUDCOMM_MANAGER, which handles
	communication with the phone.
*/
static void pipe_status_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	send_rf_debug_msg("CC Pipe Status Handler");
	if(connected) {
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
			pipes_ready = true;
			process_poll(&cloudcomm_manager);
		}
	}
}

/**
	The nrf8001 operates using data_credits. We get them when we first connect
	to the phone, and when each data transmission completes.
*/
static void data_credit_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	send_rf_debug_msg("CC Data Credit handler");
	if(connected) {
		ble_credited = true;
		sending = false;
		process_poll(&cloudcomm_manager);
	}
}

static void data_ack_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {}

static void data_received_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	send_rf_debug_msg("CC Data Received");
	if(connected) {
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
				char debug_buffer[50];
				time_t new_unixtime = 0;
				for(i=0;i<4;i++) {
					new_unixtime <<= 8;
					new_unixtime += payload[2+i];
				}
				snprintf(debug_buffer, 50, "CC Req Time %lu", new_unixtime);
				send_rf_debug_msg(debug_buffer);
				rtc_set_unixtime(new_unixtime);
			} else {
				send_rf_debug_msg("CC Other Req");
				for(i=0;i<packet_len;i++) { packet[i] = payload[i+2]; }
				callbacks[service_num](packet, packet_len);
			}
			req_count--;
			req_queue[service_num] = false;
			ble_acked = true;
			sending = false;
			process_poll(&cloudcomm_manager);
		}
		else if(pipenum == PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO) {
			if(!phone_ready) {
				phone_ready = true;
				send_rf_debug_msg("CC Phone Ready");
				process_poll(&cloudcomm_manager);
			}
			else if(payload[1] == last_sequence_num) {
				ble_acked = true;
				sending = false;
				send_rf_debug_msg("CC Ack");
				process_poll(&cloudcomm_manager);
			} else {
				send_rf_debug_msg("CC Rx Uh Oh");
				if(payload[1] == last_sequence_num - 1) {}
			}
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
		if(!sleep) {
			cancel_vtimer(&cc_failsafe_vtimer);
			cancel_vtimer(&cc_timeout_vtimer);
			sleep = true;
			bool shouldsleep = true;
			INTERRUPTS_DISABLE();
			if(connected) {
				on = false;
				shouldsleep = false;
				nrf8001_disconnect(0x01);
			}
			INTERRUPTS_ENABLE();
			if(!shouldsleep) {
				send_rf_debug_msg("BLE sleep proc nosleep");
				PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
			}
			reset_state();
			send_rf_debug_msg("Ble sleep proc");
			nrf8001_sleep();
			//PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
			leds_off(LEDS_GREEN);

			if(cc_done_callback != default_cc_callback) { send_rf_debug_msg("Ble sleep callback"); }
			else { send_rf_debug_msg("Ble sleep callback fail"); }
			(*cc_done_callback)();
			cc_done_callback = default_cc_callback;
		}
		else { send_rf_debug_msg("Ble sleep proc fail"); }
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
		send_rf_debug_msg("Ble connect proc start");
		INTERRUPTS_DISABLE();
		if(!connecting && on && (req_count > 0 || data_store_index > 0 || flash_pages_stored > 0) && device_setup_done) {
			// User has turned cloudcomm on, there is data to upload or metadata to collect, and setup is done
			if(sleep) {
				// First make sure BLE chip is awake.
				send_rf_debug_msg("CC ble_con_proc wakeup");
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
						uint8_t status = simplestore_read_next_page(&sending_data_store, max_data_store_length);
						if( status == SIMPLESTORE_SUCCESS) {
							sending_data_store_end = max_data_store_length;
							sending_data_store_empty = false;
							sending_data_store_index = 0;
							next_packet_index = cc_packet_length;
						}
						else {
							char buff[150];
							snprintf(buff, 100, "Reanding SDS Fail: %lu %u", flash_pages_stored, status);
							send_rf_debug_msg(buff);
						}
					}
				}

				connecting = true;

				if(cc_ontime != 0) {
					send_rf_debug_msg("Ble connect proc connect timeout");
					nrf8001_connect(cc_ontime/1000, 32);
				}
				else {
					send_rf_debug_msg("Ble connect proc connect no timeout");
					nrf8001_connect(0,32);
				}
			}
		}
		else if(device_setup_done) {
			// User has turned cloudcomm on but there is no data to offload
			// We make sure the BLE chip is asleep, then we call cc_done_callback
			if(!sleep) {
				send_rf_debug_msg("Ble connect proc setup sleep");
				process_poll(&ble_sleep_process);
			} else {
				send_rf_debug_msg("Ble connect proc setup?");
				leds_off(LEDS_GREEN);
				reset_state();
				cancel_vtimer(&cc_failsafe_vtimer);
				cancel_vtimer(&cc_timeout_vtimer);
				(*cc_done_callback)();
			}
		} else {
			send_rf_debug_msg("Ble proc ?");
		}
		INTERRUPTS_ENABLE();
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
	/*char buffer[100] = "ble send:";
	for(i=0;i<payload_length+1;i++) {
		snprintf(&buffer[i*2+9], 3, "%02x", sending_packet[i]);
	}
	send_rf_debug_msg(buffer); */
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
		if(failsafe) { // Check to see if our connection has gone bonkers. If so, disconnect from the phone.
			send_rf_debug_msg("CC_Manager fail sleep");
			process_poll(&ble_sleep_process);
			continue;
		} else {
			send_rf_debug_msg("CC_Manager failsafe");
			schedule_vtimer(&cc_failsafe_vtimer, VTIMER_SECOND * 20);
		}

		if(!sending && phone_ready && ble_credited && ble_acked && connected) { // first send URL to phone
			if(!urlSet && (req_count > 0 || !sending_data_store_empty)) {
				send_ble_packet(&urlIndex, metainfo.dest_len-1, (uint8_t *)metainfo.dest); // fill BLE packet with data fragment
				if(sending_packet[0] == 0xFF) {urlSet = true;}
			}
			// Deal with metainfo first
			else if(req_count > 0) {
				uint8_t i;
				char debug_buffer[50];
				snprintf(debug_buffer, 50, "CC_Man req %u", req_count);
				send_rf_debug_msg(debug_buffer);
				for(i=0;i<CLOUDCOMM_REQ_QUEUE_LENGTH;i++) {
					if(req_queue[i]) { // REQ_QUEUE keeps track of what metainfo we want.
						ble_acked = false; // lets us know when
						ble_credited = false; // lets us know when the nrf8001 can service more transmissions.
						nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX_ACK,1,&i); // send metainfo requests to phone
						break;
					}
				}

			}
			// Offload data to the phone.
			else if (!sending_data_store_empty) {
				// There is no data left in SENDING_DATA_STORE.
				// Check for more data in flash.
				// SENDING_DATA_STORE_INDEX will exceed SENDING_DATA_STORE_END because SEND_BLE_PACKET updates
				// SENDING_DATA_STORE_INDEX to the next unread index.
				send_rf_debug_msg("CC Manager data send");
				if(sending_data_store_index >= sending_data_store_end) {
					uint8_t ss_state = simplestore_read_next_page(&sending_data_store, max_data_store_length);
					sending_data_store_index = 0;
					sequence_num = 1;
					last_packet_index = 0;

					// We've successfully read more data. Continue as normal
					if(ss_state == SIMPLESTORE_SUCCESS) {
						sending_data_store_empty = false;
						sending_data_store_end = max_data_store_length;
						next_packet_index = cc_packet_length;
					}
					// No data left. clear flash chip.
					else if(ss_state == SIMPLESTORE_READ_FULL) { simplestore_clear_flash_chip(); }

					// There is no more flash data or something went wrong communicating with the flash chip.
					// Either way, disconnect and return control to other applications.s
					if(ss_state == SIMPLESTORE_FAIL || ss_state == SIMPLESTORE_READ_FULL) {
						sending_data_store_empty = true;
						sending_data_store_end = 0;
						next_packet_index = 0;
						on = false;
						process_poll(&ble_sleep_process);
						continue;
					} else {

					}
				}
				// If there is more data in the buffer, check to see if we are the end of our current data packet
				else if(sending_data_store_index == next_packet_index) {
					last_packet_index = next_packet_index;
					next_packet_index += cc_packet_length; // update data packet demarcater
				}
				else {}
				send_ble_packet(&sending_data_store_index, next_packet_index-1, &sending_data_store[0]);
			}
			else { // Nothing left to do. Time to disconnect and return control to user;
				on = false;
				send_rf_debug_msg("CC_Manager sleep");
				process_poll(&ble_sleep_process);
			}
		}
		else { send_rf_debug_msg("CC_Manager ?"); }
	}
	PROCESS_END();
}

/*************************** END CLOUDCOMM PROCESSES **************************/
static void cc_timeout_vtimer_callback() {
	on = false;
	if(!(connected && pipes_ready)) {
		send_rf_debug_msg("cc timeout sleep");
		if(!device_started) {

		}
		process_poll(&ble_sleep_process);
	} else {
		send_rf_debug_msg("cc timeout go");
	}
}
static void cc_failsafe_vtimer_callback() {
	on = false;
	failsafe = true;
	send_rf_debug_msg("cc failsafe");
	process_poll(&cloudcomm_manager);
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
	cc_timeout_vtimer = get_vtimer(cc_timeout_vtimer_callback);
	cc_failsafe_vtimer = get_vtimer(cc_failsafe_vtimer_callback);
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
   shuts down and returns control to the user. If ontime is 0, runs
   perpetually.
*/
void cloudcomm_on(void *callback, uint16_t ontime) {
	on = true; // Change state to ON
	cc_done_callback = callback; // Set callback
	send_rf_debug_msg("cc on");
	cc_ontime = ontime;
	leds_on(LEDS_GREEN);
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
	char buffer[100] = "cc store: ";
	for(i=0;i < cc_packet_length;i++) {
		data_store[data_store_index+i] = data_ptr[i]; // Copy data into DATA_STORE buffer
		snprintf(&buffer[i*2+10], 3, "%02x", data_store[data_store_index+i]);
	}
	//send_rf_debug_msg(buffer);
	data_store_index += cc_packet_length; // Update DATA_STORE write index

	// Check to see if DATA_STORE can hold another packet. If not, flush to flash.
	if( (uint32_t)data_store_index + (uint32_t)cc_packet_length > 255) {
		uint8_t data_size = data_store_index;
		flash_pages_stored += 1; // Update cloudcomm's knowledge of how much data is in flash
		data_store_index = 0; // Reset DATA_STORE
		if(simplestore_write_next_page((void *)data_store, data_size) != SIMPLESTORE_SUCCESS) {
			return 0; // Flash chip full.
		}
	}

	return 1; // everything went smoothly
}

uint8_t request_cloudcomm_data(uint8_t req) {
	if(req < CLOUDCOMM_REQ_QUEUE_LENGTH && req_queue[req] == false) {
		send_rf_debug_msg("Request cloudcomm data success");
		req_queue[req] = true;
		req_count += 1;
		return 1;
	}
	else {
		send_rf_debug_msg("Request cloudcomm data fail");
		return 0;
	}
}

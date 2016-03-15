#include "cloudcomm.h"
#include "cc2538-rf-debug.h"
#include "rtc.h"
PROCESS(ble_connect_process, "BleConnect");
PROCESS(ble_sleep_process, "BleSleep");
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

static uint8_t  sequence_num = 1;                // 1 starts a new packet to be sent to the cloud. 255 ends the packet.
static uint8_t  last_sequence_num = 0;           // Used for data ACKS from the phone
static uint8_t  sending_packet[20] = {0};        // Transmit buffer
static uint8_t  sending_data_store[270] = {0};   // Buffer where data is stored while being sent to the phone
static uint16_t sending_data_store_index = 0;    // How much data is in our sending buffer
static uint16_t next_packet_index = 0;           // Demarcates next data packet
static uint16_t last_packet_index = 0;           // Rollback index to handle unexpected disconnect
static uint16_t sending_data_store_end = 0;      // Demarcates end of valid data in our data buffer
static uint16_t max_data_store_length = 0;       // Bytes of valid data per flash page
static bool     sending_data_store_empty = true;
static bool     sent_data_from_flash = false;    // Checks if the data sent was from flash. If so, mark that page done to prevent retransmissions
static bool     use_acks = true;
static uint8_t  node_ready = 1;
// Stuff for sending over the url
static bool     urlSet = false;
static uint16_t urlIndex = 0;

//Cloudcomm operation/shutdown parameters
static void     (*cc_done_callback)();
static vtimer    cc_failsafe_vtimer;
static uint32_t  cc_ontime = 0;

//Cloudcomm ble state
static bool on = false;                // should we be scanning on ble
static bool sleep = false;
static bool connecting = false;        // currently attempting to connect
static bool connected = false;         // connected to phone
static bool pipes_ready = false;       // NRf8001 ready to do shit after connection
static bool device_setup_done = false; // nrf8001 setup complete
static bool sending = false;           // currently sending data on ble_chip
static bool ble_credited = true;       // need both this and data_acked to keep sending
static bool ble_acked = true;          // need both this and data_credited to keep sending
static bool phone_ready = false;       // Phone has to first tell us it's ready and willing
static bool tried_open_ready = false;  // Signifies that we have already tried to open the CCReady pipe
static bool tried_open_data = false;   // Signifies that we have already tried to open the CCIncomingData pipe
static bool open_pipes[NUMBER_OF_PIPES];
static bool failsafe = true;           // Used to indicate that the phone connection has gone awry
static bool device_started = false;    // When false, in sleep/setup mode. When true, in standby/connected mode.
static bool bad_state = false;         // Tracks if our wakeup command fails to actually wake the chip up.
static bool user_control_returned = false;

//Cloudcomm ready pipe byte and bit positions
static uint8_t ccra_byte_pos = 0;
static uint8_t ccra_bit_pos = 0;

//Cloudcomm incoming data pipe byte and bit positions
static uint8_t ccida_byte_pos = 0;
static uint8_t ccida_bit_pos = 0;

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

/* Get bit position for a pipe in the array returned in a pipe status event */
static uint8_t get_bit_pos(uint8_t pos) { return pos % 8; }

/* Cleanup function for cloudcomm. Resets all relevent state variables for next cc connection. */
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
	failsafe = true;
	on = false;
	sending = false;
	device_started = false;

	if(sequence_num != 1) { sending_data_store_index = last_packet_index + 1; } // if we were in the middle of a packet, go back to the start of the packet
	if(sending_data_store_index == 1) { sending_data_store_index = 0; }
	sequence_num = 1;
	last_sequence_num = 0;
}

static bool have_tasks() {
	if(req_count > 0 || data_store_index > 0 || flash_pages_stored > 0) {return true;}
	return false;
}

/*
   * Check to see if CC should initiate a BLE connection.
   * Checks that CC has been turned on by the user, CC is not currently connected or connecting, there is
   * data to upload/download, and that CC is done seting up the nrf8001
*/
static bool should_connect() {
	char buffer[100];
	snprintf(buffer, 100, "CC_SHOULD_CONN: %d %d %d %u %u %lu %d", connecting, connected, on, req_count, data_store_index, flash_pages_stored, device_setup_done);
	send_rf_debug_msg(buffer);
	if(!connecting && !connected && on && (req_count > 0 || data_store_index > 0 || flash_pages_stored > 0 || !sending_data_store_empty) && device_setup_done) {
		return true;
	}
	return false;
}

static bool cancel_cc_failsafe_vtimer() {
	if(failsafe) {
		failsafe = false;
		cancel_vtimer(&cc_failsafe_vtimer);
		return true;
	}
	return false;
}

/* Helper function to schedule failsafe timer in */
static void schedule_cc_failsafe_vtimer(uint32_t t) {
	failsafe = true;
	schedule_vtimer_ms(&cc_failsafe_vtimer, t);
}

static void return_control_to_user() {
	if(!user_control_returned) {
		user_control_returned = true;
		char buffer[100];
		snprintf(buffer, 100, "CC USER CONTROL: flash_pages_store: %lu", flash_pages_stored);
		send_rf_debug_msg(buffer);
		void (*temp)() = cc_done_callback;
		cc_done_callback = default_cc_callback;
		(*temp)();
	}
}

/*************************** NRF8001 CALLBACKS ********************************/

static void sleep_callback() { return; }

/* Handles boot conditions of NRF8001 */
static void device_started_callback(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(payload_length > 0) {
		if(payload[0] == 0x02) {
			send_rf_debug_msg("NRF8001_STARTED: setup\n");
			nrf8001_setup();
		}
		else if(payload[0] == 0x03) {
			device_setup_done = true;
			device_started = true;
			if(payload[1] == 0x00 && should_connect()) {
				send_rf_debug_msg("NRF8001_STARTED: connect\n");
				process_poll(&ble_connect_process);
			}
			else {
				send_rf_debug_msg("NRF8001_STARTED: sleep\n");
				process_poll(&ble_sleep_process);
			}
		}
		else {
			send_rf_debug_msg("NRF8001_STARTED: unknown status\n");
		}
	}
	else {
		send_rf_debug_msg("NRF8001_STARTED: unknown packet length\n");
	}
}
/**
   Indicates that we have connected to the phone. However, we can't really do
	 anything until the nrf8001 tells us it's pipes are open, so PIPE_STATUS_HANDLER
	 is functionally our connected handler.
*/
static void connected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	send_rf_debug_msg("NRF8001_CONNECTED\n");
	leds_toggle(LEDS_RED);
	connected = true;
	connecting = false;
}

static void disconnected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	INTERRUPTS_DISABLE();
	reset_state();
	if(cc_ontime == 0 && should_connect()) {
		/* User does not control to return until CC has finished processing all requests*/
		send_rf_debug_msg("NRF8001_DISCONNECTED: reconnect\n");
		on = true;
		process_poll(&ble_connect_process);
	}
	else {
		/* Normal disconnect case. Returns control to user */
		char buffer[100];
		snprintf(buffer, 100, "NRF8001_DISCONNECTED: sleep, cc_ontime: %lu, should_connect: %d", cc_ontime, should_connect());
		send_rf_debug_msg(buffer);
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
	//send_rf_debug_msg("CC Pipe Status Handler");
	if(connected) {
		if(payload[0] == 0xab) {
			pipes_ready = true;
			use_acks = true;
		}
		else if(payload[0] == 0x57) {
			pipes_ready = true;
			use_acks = false;
		}
		uint8_t i = 0;
		if(phone_ready && pipes_ready) {
			send_rf_debug_msg("NRF8001_PIPES: pipes and phone ready\n");
			for(i = 0; i < 8; i++) {
				char buffer[50];
				snprintf(buffer, 50, "Pipe block %u, %x", i, payload[i]);
				send_rf_debug_msg(buffer);
			}
			process_poll(&cloudcomm_manager);
		}
		else if(pipes_ready) {
			send_rf_debug_msg("NRF8001_PIPES: pipes ready, phone not ready");
			//if(use_acks) {nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMMETADATA_TX_ACK, 1, &node_ready);}
			//else {nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMMETADATA_TX, 1, &node_ready);}

		}
		else if(phone_ready) {
			send_rf_debug_msg("NRF8001_PIPES: phone ready, pipes not ready");
		}
		else {
			send_rf_debug_msg("NRF8001_PIPES: pipes and phone not ready");
			for(i = 0; i < 8; i++) {
				char buffer[50];
				snprintf(buffer, 50, "Pipe block %u, %x", i, payload[i]);
				send_rf_debug_msg(buffer);
			}
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
		if(ble_acked) {
			send_rf_debug_msg("NRF8001_CREDIT: connected, acked\n");
			process_poll(&cloudcomm_manager);
		}
		else {
			send_rf_debug_msg("NRF8001_CREDIT: connected, not acked\n");
		}
	}
	else {
		send_rf_debug_msg("NRF8001_DATA_CREDIT: error\n");
	}
}

static void data_ack_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {}

static void data_received_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected) {
		uint8_t pipenum = payload[0];
		if(pipenum == PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO) {
			uint8_t packet[30];
			uint8_t packet_len = payload_length-2;
			uint8_t i = 0;
			uint8_t service_num = payload[1];

			if(service_num == CLOUDCOMM_REQ_TIME) {
				send_rf_debug_msg("NRF8001_RX: Time metadata\n");
				time_t new_unixtime = 0;
				for(i=0;i<4;i++) {
					new_unixtime <<= 8;
					new_unixtime += payload[2+i];
				}
				rtc_set_unixtime(new_unixtime);
			} else {
				send_rf_debug_msg("NRF8001_RX: data\n");
				for(i=0;i<packet_len;i++) { packet[i] = payload[i+2]; }
				callbacks[service_num](packet, packet_len);
			}
			req_count--;
			req_queue[service_num] = false;
			ble_acked = true;
			sending = false;
			if(ble_credited) {
				send_rf_debug_msg("NRF8001_RX: Metadata, acked\n");
				process_poll(&cloudcomm_manager);
			}
			else {
				send_rf_debug_msg("NRF8001_RX: Metadata, no ack\n");
			}
		}
		else if(pipenum == PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO) {
			if(!phone_ready) {
				phone_ready = true;
				if(pipes_ready) {
					send_rf_debug_msg("NRF8001_RX: Phone ready, pipes ready\n");
					process_poll(&cloudcomm_manager);
				}
				else {
					send_rf_debug_msg("NRF8001_RX: Phone ready, pipes not\n");
				}
			}
			else if(payload[1] == last_sequence_num) {
				ble_acked = true;
				sending = false;
				if(last_sequence_num == 0xFF) {
					if(urlSet) {
						if(sending_data_store_index == sending_data_store_end) {
							send_rf_debug_msg("NRF8001_RX: Sending data store empty\n");
							sending_data_store_empty = true;
						}
						else {
							send_rf_debug_msg("NRF8001_RX: Next packet\n");
							last_packet_index = next_packet_index;
							next_packet_index += cc_packet_length;
						}
					}
					else {
						send_rf_debug_msg("NRF8001_RX: urlSet\n");
						urlSet = true;
					}
				}
				if(ble_credited) {
					send_rf_debug_msg("NRF8001_RX: Data acked, credited\n");
					process_poll(&cloudcomm_manager);
				}
				else {
					send_rf_debug_msg("NRF8001_RX: Data acked, no credit\n");
				}
			} else {
				send_rf_debug_msg("NRF8001_RX: error\n");
				schedule_cc_failsafe_vtimer(15000);
				bad_state = true;
				nrf8001_disconnect(0x01);
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
/*
 * Sleep managing process, ensures that the nrf8001 is in the standby state before giving the reset command.
 * Data sheet description of sleep implies that you should be able to force sleep while in connected mode. Not true.
*/
PROCESS_THREAD(ble_sleep_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		INTERRUPTS_DISABLE();
		cancel_cc_failsafe_vtimer();
		if(!sleep) {
			if(connected) {
				send_rf_debug_msg("CC_BLE_SLEEP_PROC: connected\n");
				on = false;
				bad_state = true;
				INTERRUPTS_ENABLE();
				schedule_cc_failsafe_vtimer(8000);
				nrf8001_disconnect(0x01);
				continue;
			}
			send_rf_debug_msg("CC_BLE_SLEEP_PROC: sleep\n");
			reset_state();
			sleep = true;
			nrf8001_sleep();
			INTERRUPTS_ENABLE();
			return_control_to_user();
		}
		else {
			send_rf_debug_msg("CC_BLE_SLEEP_PROC: already sleep\n");
			INTERRUPTS_ENABLE();
			return_control_to_user();
		}
	}
	PROCESS_END();
}

/*
  * Manages the BLE connection process. Basically checks that we aren't already connected
  *
  *
*/
PROCESS_THREAD(ble_connect_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		INTERRUPTS_DISABLE();
		/* Check to see if cloudcomm should attempt a BLE connection. */
		if(should_connect()) {
			if(!cancel_cc_failsafe_vtimer()) {
				send_rf_debug_msg("CC_BLE_CONNECT_PROC: failed to cancel failsafe\n");
				INTERRUPTS_ENABLE();
				continue;
			}
			/* Wake up the nrf8001 if it is asleep, which it should be */
			if(sleep) {
				send_rf_debug_msg("CC_BLE_CONNECT_PROC: asleep\n");
				sleep = false;
				schedule_cc_failsafe_vtimer(10000);
				nrf8001_wakeup();
			}
			/* Connect to the phone */
			else {
				connecting = true;
				/* Connect based on user set ontime */
				if(cc_ontime != 0) {
					send_rf_debug_msg("CC_BLE_CONNECT_PROC: limited connect\n");
					schedule_cc_failsafe_vtimer(cc_ontime * 1000 + 8000);
					nrf8001_connect(cc_ontime, 32);
				}
				/* Keep cloudcomm on until all operations are done */
				else {
					send_rf_debug_msg("CC_BLE_CONNECT_PROC: perpetual connect\n ");
					schedule_cc_failsafe_vtimer(90000);
					nrf8001_connect(60,32);
				}
			}
		}
		else {
			send_rf_debug_msg("CC_BLE_CONNECT_PROC: should not connect\n");
		}
		INTERRUPTS_ENABLE();
	}
	PROCESS_END();
}

/*************************** END NRF8001 PROCESSES ****************************/

/*************************** CLOUDCOMM PROCESSES ******************************/

static void packet_rf_debug(uint16_t payload_length, uint16_t current_data_index, uint8_t final_data_index) {
	char buffer[100] = "BLE Data Packet: ";
	char *buf_ptr = &buffer[17];
	uint8_t i = 0;
	for(i=0; i < payload_length; i++) {
		buf_ptr += sprintf(buf_ptr, "%02x", sending_packet[i]);
	}
	send_rf_debug_msg(buffer);
	snprintf(buffer, 100, "Ble Data Packet: len=%u, current_index=%u, final_index=%u\n",payload_length, current_data_index, final_data_index);
	send_rf_debug_msg(buffer);
	snprintf(buffer, 100, "Ble Data Packet: sdata_store_index=%u sdata_store_end=%u\n", sending_data_store_index, sending_data_store_end);
	send_rf_debug_msg(buffer);
}

/*
  * Defragment data packet and transmit over BLE
  * CURRENT_DATA_INDEX: current read index for our data buffer. we update this while uploading data.
  * FINAL_DATA_INDEX: ending index number of the current data packet.
  * DATA_BUFFER: Contains 1 or more packets of data ready for upload
*/
static void send_ble_packet(uint16_t *current_data_index, uint16_t final_data_index, uint8_t *data_buffer) {

	uint8_t i = 0;
	uint8_t remaining_data = final_data_index+1 - *current_data_index;
	uint8_t payload_length = remaining_data > CC_MAX_DATA_LENGTH ? CC_MAX_DATA_LENGTH : remaining_data;

	/* Check to see if this is the last fragment of the data packet */
	if(remaining_data == payload_length) {
		sending_packet[0] = 0xFF;
		sequence_num = 1;
	}
	else {
		sending_packet[0] = sequence_num;
		sequence_num += 1;
	}
	for(i=0;i<payload_length;i++) { sending_packet[i+1] = data_buffer[*current_data_index + i]; } // Load URL into ble packet

	last_sequence_num = sending_packet[0]; // keep track of sequence num for acks.
	*current_data_index += payload_length; // update current_data_index to the next UNREAD index in the data_buffer

	sending = true;
	ble_acked = false;
	ble_credited = false;
	if(use_acks) {nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK, payload_length + 1, &sending_packet[0]);}
	else {
		nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX, payload_length + 1, &sending_packet[0]);
		char buffer[50];
		snprintf(buffer, 50, "Sending CC Data: Length: %u", payload_length);
		send_rf_debug_msg(buffer);
	}

}

/* Wrapper function for send_ble_apcket() used to make sure current_data_index doesn't exceed sending_data_store_end */
static void send_cc_data_packet(uint16_t *current_data_index, uint16_t final_data_index, uint8_t *data_buffer) {
	send_ble_packet(current_data_index, final_data_index, data_buffer);
	if(*current_data_index > sending_data_store_end) {*current_data_index = sending_data_store_end;}
}

/* Transmits the URL set in metainfo to the phone */
static void transmit_url() {
	send_rf_debug_msg("CC_TRANSMIT_URL: transmit url\n");
	schedule_cc_failsafe_vtimer(8000);
	send_ble_packet(&urlIndex, metainfo.dest_len-1, (uint8_t *)metainfo.dest); // fill BLE packet with data fragment
}

/* Download metadata such as time from the phone */
static void get_metadata() {
	uint8_t i;
	for(i=0;i<CLOUDCOMM_REQ_QUEUE_LENGTH;i++) {
		if(req_queue[i]) { // REQ_QUEUE keeps track of what metainfo we want.
			send_rf_debug_msg("CC_GET_META: get metadata\n");
			ble_acked = false; // lets us know when
			ble_credited = false; // lets us know when the nrf8001 can service more transmissions.
			schedule_cc_failsafe_vtimer(10000);
			if(use_acks) { nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX_ACK,1,&i); } // send metainfo requests to phone
			else {nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX,1,&i); } // send metainfo requests to phone}

			break;
		}
	}
}

/* Use data from cloudcomm ram buffer. This is the most recent data, so we use this first. */
static void load_ble_buffer_from_ram() {
	sending_data_store_end = data_store_index - 1; // data_store_index is the first empty index in our ram buffer
	sending_data_store_empty = false;
	sending_data_store_index = 0; // reset SENDING_DATA_STORE read index

	sequence_num = 1;
	last_packet_index = 0;
	next_packet_index = cc_packet_length-1;

	data_store_index = 0;
	memcpy(&sending_data_store, &data_store, 256);
}

/* use data from flash for cloud upload. We do this in order from oldest to newest. */
static void load_ble_buffer_from_flash() {
	sending_data_store_index = 0;

	sequence_num = 1;
	last_packet_index = 0;
	next_packet_index = cc_packet_length-1;

	uint8_t status = simplestore_read_next_page(&sending_data_store, max_data_store_length);
	if(status == SIMPLESTORE_SUCCESS) {
		send_rf_debug_msg("CC_UPLOAD_DATA: load from flash success\n");
		sending_data_store_end = max_data_store_length-1;
		sending_data_store_empty = false;
		flash_pages_stored -= 1;
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
			load_ble_buffer_from_ram();
		}
		else if(flash_pages_stored > 0) {
			send_rf_debug_msg("CC_UPLOAD_DATA: load from flash\n");
			sent_data_from_flash = true;
			load_ble_buffer_from_flash();
		}
	}

	/* If our BLE buffer has data, send it out */
	if(!sending_data_store_empty) {
		char buffer[100];
		snprintf(buffer, 100, "CC_UPLOAD_DATA: upload data %u %u", sending_data_store_index, next_packet_index);
		send_rf_debug_msg(buffer);
		schedule_cc_failsafe_vtimer(10000);
		send_cc_data_packet(&sending_data_store_index, next_packet_index, sending_data_store);
	}
	/* No data left, Cloudcomm is done */
	else {
		send_rf_debug_msg("CC_UPLOAD_DATA: upload done\n");
		on = false;
		process_poll(&ble_sleep_process);
	}

}

/*
  * Manages data upload and metadata aquisition between node and smartphone.
  * Packets are fragmented into [STATUS BYTE][1-19 DATA BYTES], up to 255 bytes of data
  * STATU_BYTE == sequence number or ending byte (0xFF)
*/
PROCESS_THREAD(cloudcomm_manager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);

		INTERRUPTS_DISABLE();
		if(!cancel_cc_failsafe_vtimer()) {
			send_rf_debug_msg("CC_MANAGER: failed to cancel failsafe\n");
			INTERRUPTS_ENABLE();
			continue;
		}

		if(on && connected && !sending && phone_ready && ble_credited && ble_acked) {
			leds_toggle(LEDS_RED);
			/* Transmit upload URL to the phone. */
			if(!urlSet && have_tasks()) { transmit_url(); }
			/* Get any needed metainfo such as time */
			else if(req_count > 0) { get_metadata(); }
			/* Upload data to phone, and mark last page done if the last set of data sent was from flash */
			else if(data_store_index > 0 || flash_pages_stored > 0) {
				if(sent_data_from_flash) {
					send_rf_debug_msg("CC_MANAGER: Mark last page done");
					simplestore_mark_last_page_done();
				}
				upload_data();
			}
			/* Done all data upload/download */
			else {
				on = false;
				process_poll(&ble_sleep_process);
			}
		}
		else {
		}
		INTERRUPTS_ENABLE();
	}
	PROCESS_END();
}

/*************************** END CLOUDCOMM PROCESSES **************************/
/*
 * Fallback timer to guard against nrf8001 not waking up or not connecting correctly
*/
static void cc_failsafe_vtimer_callback() {
	if(failsafe) {
		failsafe = false;
		if(bad_state) {
			send_rf_debug_msg("CC_FAILSAFE: reset\n");
			bad_state = false;
			reset_state();
			if(cc_ontime == 0) { on = true; }
			nrf8001_reset();
			return;
		}
		send_rf_debug_msg("CC_FAILSAFE: sleep\n");
		bad_state = true;
		sleep = false;
		process_poll(&ble_sleep_process);
	}
	else {
		send_rf_debug_msg("CC_FAILSAFE: error\n");
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
	char buffer[100];
	snprintf(buffer, 100, "Cloudcomm init pages stored: %lu", flash_pages_stored);
	send_rf_debug_msg(buffer);
	cc_failsafe_vtimer = get_vtimer(cc_failsafe_vtimer_callback);
	cc_done_callback = default_cc_callback;

	process_start(&ble_connect_process, NULL);
	process_start(&cloudcomm_manager, NULL);
	process_start(&ble_sleep_process, NULL);
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

/*
  * Let cloudcomm upload data and download metadata from nearby smartphones.
  * Data upload URL is set using cloudcomm_set_info()
  * Control is returned to user once data upload/download is finished or if ontime expires
  * If ontime is 0, Cloudcomm runs perpetually until it is done uploading/downloading data.
*/
uint8_t cloudcomm_on(void *callback, uint16_t ontime) {
	cc_done_callback = callback;
	user_control_returned = false;
	/* Make sure we have data to upload or metadata to aquire, then connect */
	if(have_tasks()) {
		on = true;
		cc_ontime = ontime;
		send_rf_debug_msg("CLOUDCOMM_ON(): connect\n");
		process_poll(&ble_connect_process);
		return 1; // CC has shit to do
	}
	else {
		send_rf_debug_msg("CLOUDCOMM_ON(): return control\n");
		//return_control_to_user();
		user_control_returned = true;
		return 0; // CC HAS NOTHING TO DO
	}
}

void cloudcomm_set_metainfo(cloudcomm_meta_t *new_metainfo) {
	metainfo.dest = new_metainfo->dest;
	metainfo.dest_len = new_metainfo->dest_len;
}

void cloudcomm_set_packet_length(uint8_t len) {
	cc_packet_length = len;
	max_data_store_length = len * (255/len);
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

	/* If the buffer is full, write it to flash. Leave the last byte in the flash page free as a status byte. */
	/* We consider the buffer full when adding more data to it would overflow a flash page.                   */
	if( (uint32_t)data_store_index + (uint32_t)cc_packet_length > 255) {
		uint16_t data_size = data_store_index;
		data_store_index = 0; // Reset our ram buffer
		if(simplestore_write_next_page((void *)data_store, data_size) != SIMPLESTORE_SUCCESS) {
			send_rf_debug_msg("Cloudcomm Store Fail");
			return 0; // something went wrong
		}
		else { flash_pages_stored += 1; } // update cloudcomm's knowledge of how much data is in flash
	}

	return 1; // everything went smoothly
}

uint8_t cloudcomm_request_data(uint8_t req) {
	if(req < CLOUDCOMM_REQ_QUEUE_LENGTH && req_queue[req] == false) {
		//send_rf_debug_msg("Request cloudcomm data success");
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

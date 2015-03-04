#include "cloudcomm.h"

PROCESS(ble_connect_process, "BleConnect");
PROCESS(ble_sleep_process, "BleSleep");

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
static bool device_setup = false; // nrf8001 setup complete
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

static void sleep_callback() {
	process_poll(&ble_sleep_process);
}

static void device_started_callback(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(payload_length > 0) {
		if(payload[0] == 0x02) {
			nrf8001_setup();
		}
		else if(payload[0] == 0x03) {
			device_setup = true;
			process_poll(&ble_connect_process);
		}
	}
}

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
	if(on && (!sending_data_store_empty || req_count > 0)) {
		process_poll(&ble_connect_process);
	} else {
		sleep = true;
		process_poll(&ble_sleep_process);
	}
}

static void pipe_status_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected && on) {
		if(payload[ccra_byte_pos] & ccra_bit_pos) {}

		if(!(payload[ccra_byte_pos] & ccra_bit_pos)) {
			if(tried_open_ready == false) {
				tried_open_ready = true;
				nrf8001_open_remote_pipe(PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO);
			}
		}
		else if(!(payload[ccida_byte_pos] & ccida_bit_pos)) {
			if(tried_open_data == false) {
				tried_open_data = true;
				nrf8001_open_remote_pipe(PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO);
			}
		}
		else {
			process_poll(&cloudcomm_manager);
		}
	}
}

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
	}
	PROCESS_END();
}

PROCESS_THREAD(ble_connect_process, ev, data) {
	// Orders nrf8001 to start connection process.
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(!connecting && on && (req_count > 0 || data_store_index > 0 || flash_pages_stored > 0) && device_setup) {
			if(sleep) {
				sleep = false;
				nrf8001_wakeup();
			}
			else {
				if(sending_data_store_empty) {
					if(data_store_index > 0) {
						sending_data_store_end = data_store_index - 1;
						sending_data_store_empty = false;
						sending_data_store_index = 0;
						memcpy(&sending_data_store, &data_store, 256);
					}
					else if(flash_pages_stored > 0) {
						if(simplestore_read_next_page(&sending_data_store, max_data_store_length) == SIMPLESTORE_SUCCESS) {
							sending_data_store_end = max_data_store_length;
							sending_data_store_empty = false;
							sending_data_store_index = 0;
						}
					}
				}
				connecting = true;
				ble_ant_enable();
				nrf8001_connect(0,32);
			}
		}
		else if(device_setup) {
			sleep = true;
			nrf8001_sleep();
		}
	}
	PROCESS_END();
}

/*************************** END NRF8001 PROCESSES ****************************/

/*************************** CLOUDCOMM PROCESSES ******************************/

/*
 * Manages sending data to the phone and shutting down the ble module when we are done
 * We should already be connected to the phone.
*/
PROCESS_THREAD(cloudcomm_manager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(!sending && phone_ready && ble_credited && ble_acked && connected) {
			if(!urlSet && (req_count > 0 || !sending_data_store_empty)) {
				uint8_t i = 0;
				uint8_t remaining_data = metainfo.dest_len - urlIndex;
				uint8_t ble_packet_length = remaining_data > CLOUDCOMM_MAX_DATA_LENGTH ? CLOUDCOMM_MAX_DATA_LENGTH : remaining_data;
				if(remaining_data == ble_packet_length) {
					sending_packet[0] = 0xFF; // set sequence byte to special ending byte
					sequence_num = 1;
				}
				else {
					sending_packet[0] = sequence_num; // update sequence byte
					sequence_num += 1;
				}

				for(i=0;i<ble_packet_length;i++) {
					sending_packet[i+1] = metainfo.dest[urlIndex + i];
				}

				last_sequence_num = sending_packet[0];
				urlIndex += ble_packet_length;

				if(sending_packet[0] == 0xFF) {
					urlSet = true;
				}
				sending = true;
				ble_acked = false;
				ble_credited = false;
				nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK,
					              ble_packet_length + 1, // gotta account for sequence num
					              &sending_packet[0]);
			}
			else if(req_count > 0) {
				// Deal with requests for metainfo first
				uint8_t i = 0;
				sending = true;
				ble_acked = false;
				ble_credited = false;
				for(i=0;i<CLOUDCOMM_REQ_QUEUE_LENGTH;i++) {
					if(req_queue[i]) {
						nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX_ACK,
										  1,
										  &i);
						break;
					}
				}
			}
			else if (!sending_data_store_empty) {
				// Keep sending data until we are out of data.
				uint8_t i = 0;
				if(next_packet_index == 0) {
					next_packet_index = cc_packet_length;
				}

				uint8_t remaining_data = next_packet_index - sending_data_store_index;
				uint8_t ble_packet_length = remaining_data > CLOUDCOMM_MAX_DATA_LENGTH ? CLOUDCOMM_MAX_DATA_LENGTH : remaining_data;

				if(remaining_data == ble_packet_length) {
					sending_packet[0] = 0xFF; // set sequence byte to special ending byte
					sequence_num = 1;
				}
				else {
					sending_packet[0] = sequence_num; // update sequence byte
					sequence_num += 1;
				}

				last_sequence_num = sending_packet[0];

				for(i=0;i<ble_packet_length;i++) {
					sending_packet[i+1] = sending_data_store[sending_data_store_index+i];
				}

				sending_data_store_index += ble_packet_length;

				if(sending_data_store_index >= sending_data_store_end) {
					// Current data store has been exhausted, check flash chip for more data
					uint8_t ss_state = simplestore_read_next_page(&sending_data_store, max_data_store_length);
					sending_data_store_index = 0;
					sequence_num = 1;
					next_packet_index = 0;

					if(ss_state == SIMPLESTORE_SUCCESS) {
						sending_data_store_empty = false;
						sending_data_store_end = max_data_store_length;
					}
					else if(ss_state == SIMPLESTORE_FULL) {
						sending_data_store_empty = true;
						sending_data_store_end = 0;
					}
					else if(ss_state == SIMPLESTORE_FAIL) {
						sending_data_store_empty = true;
						sending_data_store_end = 0;
					}
				}
				else if(sending_data_store_index == next_packet_index) {
					// update packet end byte
					next_packet_index += cc_packet_length;
				}

				sending = true;
				ble_acked = false;
				ble_credited = false;
				nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK,
					              ble_packet_length + 1, // gotta account for sequence num
					              &sending_packet[0]);
			}
			else {
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

void cloudcomm_on(void *callback, uint16_t ontime) {
	on = true;
	cc_done_callback = callback;
	schedule_vtimer(&cc_vtimer, VTIMER_SECOND/1000 * ontime);
	process_poll(&ble_connect_process);
}

void cloudcomm_set_metainfo(cloudcomm_meta_t *new_metainfo) {
	metainfo.dest = new_metainfo->dest;
	metainfo.dest_len = new_metainfo->dest_len;
}

void cloudcomm_set_packet_length(uint8_t len) {
	cc_packet_length = len;
	max_data_store_length = len * (256/len);
}

uint8_t cloudcomm_store(void *data) {
	uint8_t i = 0;
	uint8_t *data_ptr = (uint8_t *)data;
	if(data_store_index + cc_packet_length < 256) {
		for(i=0;i < cc_packet_length;i++) {
			data_store[data_store_index+i] = data_ptr[i];
		}
		data_store_index += cc_packet_length;
	}
	else {
		if(simplestore_write_next_page((void *)&data_store, data_store_index) != SIMPLESTORE_SUCCESS) {
			return 0;
		}
		flash_pages_stored += 1;
		data_store_index = 0;
		for(i=0;i<cc_packet_length;i++) {
			data_store[data_store_index+i] = data_ptr[i];
		}
		data_store_index += cc_packet_length;
	}
	return 1;
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

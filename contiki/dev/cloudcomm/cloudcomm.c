#include "cloudcomm.h"

PROCESS(ble_setup_process, "BleSetup");
PROCESS(ble_connect_process, "BleConnect");
PROCESS(ble_pipe_process, "BlePipe");
PROCESS(ble_credit_process, "BleCredit");

PROCESS(cloudcomm_data_manager, "CCData");
PROCESS(cloudcomm_req_manager, "CCReqs");

static cloudcomm_meta_t metadata;
static cloudcomm_data_t *current_data = NULL;
static cloudcomm_data_t *data_head = NULL;
static cloudcomm_req_t *current_req = NULL;
static cloudcomm_req_t *req_head = NULL;

static bool on = false; // should we be scanning on ble
static bool connecting = false;
static bool connected = false; // ble connection status;
static bool pending_data = false;  // have data to send to the cloud
static bool pending_requests = false; // have requests for data
static bool device_setup = false; // nrf8001 setup complete
static bool in_use = false; // currently using ble_chip
static bool pipe_states[NUMBER_OF_PIPES];

static void default_callback(uint8_t payload_length, uint8_t payload[30]) {}

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
static void device_started_callback(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(payload_length > 0) {
		if(payload[0] == 0x02) {
			process_poll(&ble_setup_process);
		}
		else if(payload[0] == 0x03) {
			device_setup = true;
			if(on && (pending_data || pending_requests)) {
				process_poll(&ble_connect_process);
			}
		}
	}
}

static void connected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	connected = true;
}

static void disconnected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	connected = false;
	if(on && (pending_data || pending_requests)) {
		process_poll(&ble_connect_process);
	}
}

static void pipe_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected == true) {
		if(on) {
			if(pending_requests) {
				if ( payload[get_byte_pos(PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX)]
					 & get_bit_pos(PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX) ) {
					 	process_poll(&cloudcomm_req_manager);
				}
			}
			else if(pending_data) {
				if ( payload[get_byte_pos(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX)]
					 & get_bit_pos(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX) ) {
					 	process_poll(&cloudcomm_data_manager);
				}
			}
		}
	}
}

static void data_credit_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected && on) {
		if(pending_requests) {
			if(current_req->next == NULL) {
				free(current_req);
				current_req = NULL;
				req_head = NULL;
				pending_requests = false;
				if(pending_data) {
					cloudcomm_data_t *temp = current_data
					process_poll(&cloudcomm_data_manager);
				}
			}
			else {
				cloudcomm_req_t *temp = current_req->next;
				free(current_req);
				current_req = temp;
				temp = NULL;
				process_poll(&cloudcomm_req_manager);
			}
		}
		else if(pending_data) {
			if(current_data->next == NULL) {
				free(current_data);
			}
			cloudcomm_data_t *temp = current_data
			process_poll(&cloudcomm_data_manager);
		}
	}
}

static void data_received_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	uint8_t pipenum = payload[0];
}

/*************************** END NRF8001 CALLBACKS ****************************/

/******************************* NRF8001 PROCESSES ****************************/

PROCESS_THREAD(ble_setup_process, ev, data) {
	// Setup the BLE chip
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		nrf8001_setup();
	}
	PROCESS_END();
}

PROCESS_THREAD(ble_connect_process, ev, data) {
	// Orders nrf8001 to start connection process.
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(connecting == false) {
			connecting = true;
			ble_ant_enable();
			nrf8001_connect(0,32);
		}
		
	}
	PROCESS_END();
}

PROCESS_THREAD(ble_pipe_process, ev, data) {
	// Lets us know what types of data we can receive or transmit
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
	}
	PROCESS_END();
}

PROCESS_THREAD(ble_credit_process, ev, data) {
	// lets us know that we can keep transmitting
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
	}
	PROCESS_END();
}

/*************************** END NRF8001 PROCESSES ****************************/

/*************************** CLOUDCOMM PROCESSES ******************************/

PROCESS_THREAD(cloudcomm_data_manager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
	}
	PROCESS_END();
}

PROCESS_THREAD(cloudcomm_req_manager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
	}
	PROCESS_END();
}

/*************************** END CLOUDCOMM PROCESSES **************************/

void cloudcomm_init() {
	process_start(&ble_connect_process, NULL);
	process_start(&ble_setup_process, NULL);
	process_start(&ble_pipe_process, NULL);
	process_start(&ble_connect_process, NULL);
	process_start(&cloudcomm_data_manager, NULL);
	process_start(&cloudcomm_req_manager, NULL);
	nrf8001_register_callback(NRF8001_DEVICE_STARTED_EVENT, (nrf8001_callback_t) device_started_callback);
	nrf8001_register_callback(NRF8001_CONNECTED_EVENT, (nrf8001_callback_t) connected_handler);
	nrf8001_register_callback(NRF8001_DISCONNECTED_EVENT, (nrf8001_callback_t) disconnected_handler);
	nrf8001_register_callback(NRF8001_PIPE_STATUS_EVENT, (nrf8001_callback_t) pipe_handler);
	nrf8001_register_callback(NRF8001_DATA_CREDIT_EVENT, (nrf8001_callback_t) data_credit_handler);
	nrf8001_register_callback(NRF8001_DATA_RECEIVED_EVENT, (nrf8001_callback_t) data_received_handler);
	nrf8001_enable();
}

void start_cloudcomm() {
	on = true;
	if((pending_data || pending_requests) && device_setup) {
		process_poll(&ble_connect_process);
	} 
}

void stop_cloudcomm() {
	on = false;
	connected = false;
	nrf8001_disconnect(0x01);
}

void set_cloudcomm_metainfo(cloudcomm_meta_t metainfo) {
	uint8_t i;
	for(i=0;i<8;i++) {
		metadata.ip[i] = metainfo.ip[i];
	}
}

void send_cloudcomm_data(void *data, size_t len) {
	if(current_data == NULL && data_head == NULL) {
		current_data = (cloudcomm_data_t *)malloc(sizeof(cloudcomm_data_t));
		current_data->data = data;
		current_data->len = len;
		current_data->current_byte = 0;
		current_data->next = NULL;
		data_head = current_data;
	} else {
		data_head->next = (cloudcomm_data_t *)malloc(sizeof(cloudcomm_data_t));
		data_head = data_head->next;
		data_head->data = data;
		data_head->len = len;
		data_head->current_byte = 0;
		data_head->next = NULL;
	}
	pending_data = true;
	if(on && device_setup) {
		if(connected) {
			process_poll(&cloudcomm_data_manager);
		} else {
			process_poll(&ble_connect_process);
		}
	}
}

void request_cloudcomm_data(uint8_t req) {
	if(current_req == NULL && req_head == NULL) {
		current_req = (cloudcomm_req_t *)malloc(sizeof(cloudcomm_req_t));
		current_req->req = req;
		current_req->next = NULL;
		req_head = current_req;
	} else {
		req_head->next = (cloudcomm_req_t *)malloc(sizeof(cloudcomm_req_t));
		req_head = req_head->next;
		req_head->req = req;
		req_head->next = NULL;
	}
	pending_requests = true;
	if(on && device_setup) {
		if(connected) {
			process_poll(&cloudcomm_req_manager);
		} else {
			process_poll(&ble_connect_process);
		}
	}
}

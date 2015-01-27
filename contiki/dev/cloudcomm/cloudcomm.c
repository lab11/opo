#include "cloudcomm.h"

PROCESS(ble_setup_process, "BleSetup");
PROCESS(ble_connect_process, "BleConnect");
PROCESS(ble_pipe_process, "BlePipe");
PROCESS(ble_credit_process, "BleCredit");

PROCESS(cloudcomm_manager, "CCData");

static cloudcomm_meta_t metadata;
static cloudcomm_data_t *current_data = NULL;
static cloudcomm_data_t *data_head = NULL;
static cloudcomm_req_t *current_req = NULL;
static cloudcomm_req_t *req_head = NULL;

static bool on = false; // should we be scanning on ble
static bool connecting = false;
static bool connected = false; // ble connection status;
static bool device_setup = false; // nrf8001 setup complete
static bool sending = false; // currently sending data on ble_chip
static bool ble_credited = false; // need both this and data_acked to keep sending
static bool ble_acked = false; // need both this and data_credited to keep sending
static bool open_pipes[NUMBER_OF_PIPES];

static cloudcomm_callback_t callbacks[CLOUDCOMM_REQ_SIZE];

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
static void device_started_callback(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(payload_length > 0) {
		if(payload[0] == 0x02) {
			process_poll(&ble_setup_process);
		}
		else if(payload[0] == 0x03) {
			device_setup = true;
			if(on && (current_data != NULL || current_req != NULL)) {
				process_poll(&ble_connect_process);
			}
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
	if(on && (current_data != NULL || current_req != NULL)) {
		process_poll(&ble_connect_process);
	}
}

static void pipe_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected && on) {
		process_poll(&cloudcomm_manager);
	}
}

static void data_credit_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected && on) {
		ble_credited = true;
		process_poll(&cloudcomm_manager);
	}
}

static void data_ack_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	INTERRUPTS_DISABLE();
	ble_acked = true;
	if(current_req != NULL) {
		if(current_req->next == NULL) {
			req_head = NULL;
			free(current_req);
			current_req = NULL;
		} else {
			cloudcomm_req_t *temp = current_req->next;
			free(current_req);
			current_req = temp;
			temp = NULL;
		}
	}
	else if(current_data != NULL) {
		if(current_data->len - current_data->current_byte > CLOUDCOMM_MAX_DATA_LENGTH) {
			current_data->current_byte += CLOUDCOMM_MAX_DATA_LENGTH;
		} else {
			if(current_data->next == NULL) {
				data_head = NULL;
				free(current_data);
				current_data = NULL;
			} else {
				cloudcomm_data_t *temp = current_data->next;
				free(current_data);
				current_data = temp;
				temp = NULL;
			}
		}
	}
	INTERRUPTS_ENABLE();
	process_poll(&cloudcomm_manager);
}

static void data_received_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	uint8_t pipenum = payload[0];
	if(pipenum == PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO) {
		uint8_t packet[30];
		uint8_t packet_len = payload_length-2;
		uint8_t i = 0;
		uint8_t service_num = payload[1];
		for(i=0;i<packet_len;i++) {
			packet[i] = payload[i+2];
		}
		callbacks[service_num](packet, packet_len);
	}
	

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
		if(!connecting && on && (current_req != NULL || current_data != NULL)) {
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

PROCESS_THREAD(cloudcomm_manager, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		if(!connected && !connecting) {
			process_poll(&ble_connect_process);
		}
		else if(!sending) {
			if(current_req != NULL) {

			}
			else if (current_data != NULL) {
				uint16_t remaining_data = current_data->len - current_data->current_byte;
				uint8_t packet_length = remaining_data > CLOUDCOMM_MAX_DATA_LENGTH ? CLOUDCOMM_MAX_DATA_LENGTH : current_data->len;
				uint8_t *data_ptr = (uint8_t *)current_data->data;
				sending = true;
				nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK,
					              packet_length,
					              &data_ptr[current_data->current_byte]);
			}
		}
	}
	PROCESS_END();
}

/*************************** END CLOUDCOMM PROCESSES **************************/

void cloudcomm_init() {
	uint8_t i = 0;
	for(i=0;i<CLOUDCOMM_REQ_SIZE;i++) {
		callbacks[i] = (cloudcomm_callback_t) default_callback;
	}
	for(i=0;i<NUMBER_OF_PIPES;i++) {
		open_pipes[i] = false;
	}

	process_start(&ble_connect_process, NULL);
	process_start(&ble_setup_process, NULL);
	process_start(&ble_pipe_process, NULL);
	process_start(&ble_connect_process, NULL);
	process_start(&cloudcomm_manager, NULL);
	nrf8001_register_callback(NRF8001_DEVICE_STARTED_EVENT, (nrf8001_callback_t) device_started_callback);
	nrf8001_register_callback(NRF8001_CONNECTED_EVENT, (nrf8001_callback_t) connected_handler);
	nrf8001_register_callback(NRF8001_DISCONNECTED_EVENT, (nrf8001_callback_t) disconnected_handler);
	nrf8001_register_callback(NRF8001_PIPE_STATUS_EVENT, (nrf8001_callback_t) pipe_handler);
	nrf8001_register_callback(NRF8001_DATA_CREDIT_EVENT, (nrf8001_callback_t) data_credit_handler);
	nrf8001_register_callback(NRF8001_DATA_RECEIVED_EVENT, (nrf8001_callback_t) data_received_handler);
	nrf8001_register_callback(NRF8001_DATA_ACK_EVENT, (nrf8001_callback_t) data_ack_handler);
	nrf8001_enable();
}

void start_cloudcomm() {
	on = true;
	process_poll(&ble_connect_process);
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
	INTERRUPTS_DISABLE();
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
	INTERRUPTS_ENABLE();
	process_poll(&cloudcomm_manager);
}

void request_cloudcomm_data(uint8_t req) {
	INTERRUPTS_DISABLE();
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
	INTERRUPTS_ENABLE();
	process_poll(&cloudcomm_manager);
}

#include "cloudcomm.h"

PROCESS(ble_setup_process, "BleSetup");
PROCESS(ble_connect_process, "BleConnect");
PROCESS(ble_pipe_process, "BlePipe");
PROCESS(ble_credit_process, "BleCredit");
PROCESS(ble_sleep_process, "BleSleep");

PROCESS(cloudcomm_manager, "CCData");

static cloudcomm_meta_t metadata;
static int data_index = -1;
static int data_head = -1;
static uint8_t req_count = 0;
static cloudcomm_data_t data_queue[CLOUDCOMM_DATA_QUEUE_LENGTH];
static bool req_queue[CLOUDCOMM_REQ_QUEUE_LENGTH];

static bool on = false; // should we be scanning on ble
static bool connecting = false;
static bool connected = false; // ble connection status;
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
static uint8_t ccr_byte_pos = 0;
static uint8_t ccr_bit_pos = 0;

//Cloudcomm incoming data pipe byte and bit positions
static uint8_t ccida_byte_pos = 0;
static uint8_t ccida_bit_pos = 0;
static uint8_t ccid_byte_pos = 0;
static uint8_t ccid_bit_pos = 0;

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
			process_poll(&ble_setup_process);
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
	if(on && (data_index > -1 || req_count > 0)) {
		process_poll(&ble_connect_process);
	} else {
		nrf8001_sleep();
	}
}

static void pipe_status_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected && on) {
		if(payload[ccra_byte_pos] & ccra_bit_pos) {
			//leds_on(LEDS_GREEN);
		}
		if(payload[ccr_byte_pos] & ccr_bit_pos) {
			//leds_on(LEDS_BLUE);
		}
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
		process_poll(&cloudcomm_manager);
	}
}

static void data_credit_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected && on) {
		ble_credited = true;
		sending = false;
		//leds_on(LEDS_RED);
		process_poll(&cloudcomm_manager);
	}
}

static void data_ack_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	INTERRUPTS_DISABLE();
	ble_acked = true;
	if(data_index > -1 && req_count == 0) {
		if(data_queue[data_index].len - data_queue[data_index].current_byte > CLOUDCOMM_MAX_DATA_LENGTH) {
			data_queue[data_index].current_byte += CLOUDCOMM_MAX_DATA_LENGTH;
		} else {
			if(data_index == data_head) {
				data_index = -1;
				data_head = -1;
			} else {
				data_index++;
			}
		}
	}
	INTERRUPTS_ENABLE();
	//leds_on(LEDS_GREEN);
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
		req_count--;
		req_queue[service_num] = false;
		callbacks[service_num](packet, packet_len);
	}
	else if(pipenum == PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO || pipenum == PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX) {
		phone_ready = true;
		process_poll(&cloudcomm_manager);
	}
}

static void pipe_error_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(payload[0] == PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK) {
		uint8_t ecode = payload[1];
		if(ecode == ACI_STATUS_ERROR_DATA_SIZE) {
			//leds_on(LEDS_BLUE);
		}
		else if(ecode == ACI_STATUS_ERROR_PIPE_INVALID) {
			//leds_on(LEDS_GREEN);
		}
		else if(ecode == ACI_STATUS_ERROR_CREDIT_NOT_AVAILABLE) {
			//leds_on(LEDS_RED);
		}
		else if(ecode == ACI_STATUS_ERROR_PEER_ATT_ERROR) {
			//leds_on(LEDS_RED);
			//leds_on(LEDS_BLUE);
		}
		else if(ecode == ACI_STATUS_ERROR_PIPE_TYPE_INVALID) {
			//leds_on(LEDS_BLUE);
		}
		else if(ecode == ACI_STATUS_ERROR_PIPE_STATE_INVALID) {
			//leds_on(LEDS_GREEN);
		}
		else {
			//leds_on(LEDS_BLUE);
			//leds_on(LEDS_GREEN);
		}
	}
}

/*************************** END NRF8001 CALLBACKS ****************************/

/******************************* NRF8001 PROCESSES ****************************/
PROCESS_THREAD(ble_sleep_process, ev, data) {
	// Setup the BLE chip
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
	}
	PROCESS_END();
}

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
		if(!connecting && on && (req_count > 0 || data_index > -1) && device_setup) {
			connecting = true;
			ble_ant_enable();
			nrf8001_connect(0,32);
		}
		else if(device_setup) {
			nrf8001_sleep();
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
		else if(!sending && phone_ready && ble_credited && ble_acked) {
			if(req_count > 0) {
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
			else if (data_index > -1) {
				uint16_t remaining_data = data_queue[data_index].len - data_queue[data_index].current_byte;
				uint8_t packet_length = remaining_data > CLOUDCOMM_MAX_DATA_LENGTH ? CLOUDCOMM_MAX_DATA_LENGTH : data_queue[data_index].len;
				sending = true;
				ble_acked = false;
				ble_credited = false;
				nrf8001_send_data(PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK,
					              packet_length,
					              &data_queue[data_index].data[data_queue[data_index].current_byte]);
			}
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
	for(i=0;i<NUMBER_OF_PIPES;i++) {
		open_pipes[i] = false;
	}

	ccra_byte_pos = get_byte_pos(PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO);
	ccra_bit_pos = get_bit_pos(PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO);
	ccr_byte_pos = get_byte_pos(PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX);
	ccr_bit_pos = get_bit_pos(PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX);

	ccida_byte_pos = get_byte_pos(PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO);
	ccida_bit_pos = get_bit_pos(PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO);
	ccid_byte_pos = get_byte_pos(PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX);
	ccid_bit_pos = get_bit_pos(PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX);

	process_start(&ble_connect_process, NULL);
	process_start(&ble_setup_process, NULL);
	process_start(&ble_pipe_process, NULL);
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
	GPIO_POWER_UP_ON_FALLING(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN_MASK);
	GPIO_ENABLE_POWER_UP_INTERRUPT(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN_MASK);
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

uint8_t send_cloudcomm_data(void *data, uint8_t len) {
	uint8_t result = 0;
	INTERRUPTS_DISABLE();
	if(len <= CLOUDCOMM_MAX_DATA_SIZE) {
		if(++data_head < CLOUDCOMM_DATA_QUEUE_LENGTH) {
			uint8_t i = 0;
			uint8_t *data_ptr = (uint8_t *) data;
			data_queue[data_head].len = len;
			data_queue[data_head].current_byte = 0;
			for(i=0;i<len;i++) {
				data_queue[data_head].data[i] = data_ptr[i];
			}
			result = 1;
			if(data_index == -1) {
				data_index = 0;
			}
		} else {
			data_head--;
		}
	}
	INTERRUPTS_ENABLE();
	process_poll(&cloudcomm_manager);
	return result;
}

uint8_t request_cloudcomm_data(uint8_t req) {
	uint8_t result = 1;
	INTERRUPTS_DISABLE();
	if(req < CLOUDCOMM_REQ_QUEUE_LENGTH) {
		req_queue[req] = true;
		req_count++;
	} else {
		result = 0;
	}
	INTERRUPTS_ENABLE();
	process_poll(&cloudcomm_manager);
	return result;
}

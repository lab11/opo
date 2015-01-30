#include "ble-test.h"
#include "nrf8001.h"
#include "contiki.h"
#include "nrf8001.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "rf_switch.h"
#include "dev/leds.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "nrf8001_setup.h"
#include <stdbool.h>

/*---------------------------------------------------------------------------*/
PROCESS(ble_test, "BleTest");
PROCESS(ble_setup_process, "BleSetup");
PROCESS(ble_connect_process, "BleSetup");
PROCESS(ble_pipe_process, "BlePipe");
PROCESS(ble_credit_process, "BleCredit");
AUTOSTART_PROCESSES(&ble_test);
/*---------------------s------------------------------------------------------*/

uint8_t open_pipes[64] = {0};
uint8_t test_data[6] = {65,65,65,65,65,65};
nrf8001_event_packet ep = {0};
int msg_count = 0;
//static struct etimer et;
bool connected = false;
bool time_set = false;
bool tx_start = false;
opo_ble_t test;

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

static void device_started_callback(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(payload_length > 0) {
		if(payload[0] == 0x02) {
			process_poll(&ble_setup_process);
		}
		else if(payload[0] == 0x03) {
			process_poll(&ble_connect_process);
		}
		else {
			//leds_on(LEDS_RED);
		}
	}
}

static void connected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	connected = true;
}

static void disconnected_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	connected = false;
	process_poll(&ble_connect_process);
}

static void pipe_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	if(connected == true) {
		uint8_t byte_pos = get_byte_pos(PIPE_OPOV4_OPODATA_TX);
		uint8_t bit_pos = get_bit_pos(PIPE_OPOV4_OPODATA_TX);
		if (payload[byte_pos] & bit_pos) {
			if(time_set == true) {
				nrf8001_send_data(PIPE_OPOV4_OPODATA_TX,
						  		  sizeof(test),
						          (uint8_t *) &test);
			} else {
				tx_start = true;
			}

		} else {
			leds_on(LEDS_GREEN);
		}
	}
}

static void data_credit_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	test.tx_id += 1;
	if (test.tx_id < 50) {
		nrf8001_send_data(PIPE_OPOV4_OPODATA_TX,
						  sizeof(test),
						  (uint8_t *) &test);
	}
}

static void data_received_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {
	uint8_t pipenum = payload[0];
	if(pipenum == PIPE_OPOV4_UNIXTIME_RX || pipenum == PIPE_OPOV4_UNIXTIME_RX_ACK_AUTO) {
		uint8_t i = 0;
		if(payload_length == 5) {
			leds_on(LEDS_GREEN);
			for(i=1; i<payload_length; i++) {
				test.m_full_time <<= 8;
				test.m_full_time += payload[i];
			}
			time_set = true;
			if(tx_start == true) {
				nrf8001_send_data(PIPE_OPOV4_OPODATA_TX,
						  		  sizeof(test),
						          (uint8_t *) &test);
			}
		}
		else if(payload_length > 5) {
			leds_on(LEDS_BLUE);
		}
		else {
			leds_on(LEDS_RED);
		}
	}
}


void sfd_callback() {}

void txdone_callback() {}

void rfrx_callback() {}

PROCESS_THREAD(ble_setup_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		nrf8001_setup();
	}
	PROCESS_END();
}

PROCESS_THREAD(ble_connect_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		nrf8001_connect(0,32);
	}
	PROCESS_END();
}

PROCESS_THREAD(ble_pipe_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		nrf8001_send_data(PIPE_OPOV4_OPODATA_TX,
						  sizeof(test),
						  (uint8_t *) &test);
	}
	PROCESS_END();
}

PROCESS_THREAD(ble_credit_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		nrf8001_send_data(PIPE_OPOV4_OPODATA_TX,
						  sizeof(test),
						  (uint8_t *) &test);
	}
	PROCESS_END();
}

PROCESS_THREAD(ble_test, ev, data) {
	PROCESS_BEGIN();
	SFD_HANDLER.set_callback(sfd_callback);
  	RF_TXDONE_HANDLER.set_callback(txdone_callback);
  	simple_network_set_callback(rfrx_callback);
	ble_ant_enable();
	NETSTACK_MAC.on();

	test.tx_id = 1;
    test.rx_id = 2;
    test.ultrasonic_rf_dt = 3000;
    test.ultrasonic_dt = 200;
    test.m_full_time = 45;
    test.tx_full_time = 20;
    test.m_reset_counter = 10;
    test.tx_reset_counter = 11;

	process_start(&ble_connect_process, NULL);
	process_start(&ble_setup_process, NULL);
	process_start(&ble_pipe_process, NULL);
	process_start(&ble_connect_process, NULL);
	nrf8001_register_callback(NRF8001_DEVICE_STARTED_EVENT, (nrf8001_callback_t) device_started_callback);
	nrf8001_register_callback(NRF8001_CONNECTED_EVENT, (nrf8001_callback_t) connected_handler);
	nrf8001_register_callback(NRF8001_DISCONNECTED_EVENT, (nrf8001_callback_t) disconnected_handler);
	nrf8001_register_callback(NRF8001_PIPE_STATUS_EVENT, (nrf8001_callback_t) pipe_handler);
	nrf8001_register_callback(NRF8001_DATA_CREDIT_EVENT, (nrf8001_callback_t) data_credit_handler);
	nrf8001_register_callback(NRF8001_DATA_RECEIVED_EVENT, (nrf8001_callback_t) data_received_handler);

	nrf8001_enable();

	PROCESS_END();
}

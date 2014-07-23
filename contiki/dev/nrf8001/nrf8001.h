#ifndef NRF8001_H
#include "lib/sensors.h"
#define NRF8001_H

// System Command Op Codes
#define NRF8001_TEST 0x01
#define NRF8001_ECHO 0x02
#define NRF8001_DTM_COMMAND 0x03
#define NRF8001_SLEEP 0x04
#define NRF8001_WAKEUP 0x05
#define NRF8001_SETUP 0x06
#define NRF8001_READ_DYNAMIC_DATA 0x07
#define NRF8001_WRITE_DYNAMIC_DATA 0x08
#define NRF8001_GET_DEVICE_VERSION 0x09
#define NRF8001_GET_DEVICE_ADDRESS 0x0A
#define NRF8001_GET_BATTERY_LEVEL 0x0B
#define NRF8001_GET_TEMPERATURE 0x0C
#define NRF8001_RADIO_RESET 0x0E
#define NRF8001_CONNECT 0x0F
#define NRF8001_BOND 0x10
#define NRF8001_DISCONNECT 0x11
#define NRF8001_SET_TX_POWER 0x12
#define NRF8001_CHANGE_TIMING_REQUEST 0x13
#define NRF8001_OPEN_REMOTE_PIPE 0x14
#define NRF8001_SET_APPLICATION_LATENCY 0x19
#define NRF8001_SET_KEY 0x1A
#define NRF8001_OPEN_ADV_PIPE 0x1B
#define NRF8001_BROADCAST 0x1C
#define NRF8001_BOND_SEC_REQUEST 0x1D
#define NRF8001_DIRECTED_CONNECT 0x1E
#define NRF8001_CLOSE_REMOTE_PIPE 0x1F

// System Events Op Codes
#define NRF8001_DEVICE_STARTED_EVENT 0x81
#define NRF8001_ECHO_EVENT 0x82
#define NRF8001_HARDWARE_ERROR_EVENT 0x83
#define NRF8001_COMMAND_RESPONSE_EVENT 0x84
#define NRF8001_CONNECTED_EVENT 0x85
#define NRF8001_DISCONNECTED_EVENT 0x86
#define NRF8001_BOND_STATUS_EVENT 0x87
#define NRF8001_PIPE_STATUS_EVENT 0x88
#define NRF8001_TIMING_EVENT 0x89
#define NRF8001_DISPLAY_KEY_EVENT 0x8E
#define NRF8001_KEY_REQUEST_EVENT 0x8F

// Data commands
#define NRF8001_SET_LOCAL_DATA 0x0D
#define NRF8001_SEND_DATA 0x15
#define NRF8001_SEND_DATA_ACK 0x16
#define NRF8001_REQUEST_DATA 0x17
#define NRF8001_SEND_DATA_NACK 0x18

// Data events
#define NRF8001_DATA_CREDIT_EVENT 0x8A
#define NRF8001_DATA_ACK_EVENT 0x8B
#define NRF8001_DATA_RECEIVED_EVENT 0x8C
#define NRF8001_PIPE_ERROR_EVENT 0x8D


// NRF event packet
typedef struct{
	uint8_t length;
	uint8_t event;
	uint8_t packet[30];
} nrf8001_event_packet ;

typedef struct{
	uint8_t length;
	uint8_t command;
	uint8_t packet[30];
} nrf8001_command_packet;

typedef struct {
	uint8_t debug;
	uint8_t payload[0x30];
} nrf8001_setup_msg_t;


#define NRF8001_SENSOR "NRF8001_SENSOR"
extern const struct sensors_sensor nrf8001_event;

void nrf8001_init();
void nrf8001_enable();
void nrf8001_test(uint8_t test_type);
void nrf8001_echo(uint8_t packet_length, uint8_t *packet);
void nrf8001_setup(nrf8001_setup_msg_t packet);
void nrf8001_conneect(uint16_t timeout, uint16_t advertising);
nrf8001_event_packet nrf8001_get_event();

#endif

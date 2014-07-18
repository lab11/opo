#ifndef NRF8001_H
#include "lib/sensors.h"
#define NRF8001_H

// System Command Op Codes
#define TEST 0x01
#define ECHO 0x02
#define DTM_COMMAND 0x03
#define SLEEP 0x04
#define WAKEUP 0x05
#define SETUP 0x06
#define READ_DYNAMIC_DATA 0x07
#define WRITE_DYNAMIC_DATA 0x08
#define GET_DEVICE_VERSION 0x09
#define GET_DEVICE_ADDRESS 0x0A
#define GET_BATTERY_LEVEL 0x0B
#define GET_TEMPERATURE 0x0C
#define RADIO_RESET 0x0E
#define CONNECT 0x0F
#define BOND 0x10
#define DISCONNECT 0x11
#define SET_TX_POWER 0x12
#define CHANGE_TIMING_REQUEST 0x13
#define OPEN_REMOTE_PIPE 0x14
#define SET_APPLICATION_LATENCY 0x19
#define SET_KEY 0x1A
#define OPEN_ADV_PIPE 0x1B
#define BROADCAST 0x1C
#define BOND_SEC_REQUEST 0x1D
#define DIRECTED_CONNECT 0x1E
#define CLOSE_REMOTE_PIPE 0x1F

// System Events Op Codes
#define DEVICE_STARTED_EVENT 0x81
#define ECHO_EVENT 0x82
#define HARDWARE_ERROR_EVENT 0x83
#define COMMAND_RESPONSE_EVENT 0x84
#define CONNECTED_EVENT 0x85
#define DISCONNECTED_EVENT 0x86
#define BOND_STATUS_EVENT 0x87
#define PIPE_STATUS_EVENT 0x88
#define TIMING_EVENT 0x89
#define DISPLAY_KEY_EVENT 0x8E
#define KEY_REQUEST_EVENT 0x8F

// Data commands
#define SET_LOCAL_DATA 0x0D
#define SEND_DATA 0x15
#define SEND_DATA_ACK 0x16
#define REQUEST_DATA 0x17
#define SEND_DATA_NACK 0x18

// Data events
#define DATA_CREDIT_EVENT 0x8A
#define DATA_ACK_EVENT 0x8B
#define DATA_RECEIVED_EVENT 0x8C
#define PIPE_ERROR_EVENT 0x8D


// NRF event packet
typedef struct{
	uint8_t event;
	uint8_t length;
	uint8_t packet[30];
} nrf8001_event_packet ;

typedef struct{
	uint8_t length;
	uint8_t command;
	uint8_t packet[30];
} nrf8001_command_packet;

#define NRF8001_SENSOR "NRF8001_SENSOR"
extern const struct sensors_sensor nrf8001_event;

void nrf8001_init();
void nrf8001_enable();
void nrf8001_test(uint8_t test_type);
void nrf8001_echo(uint8_t packet_length, uint8_t *packet);
nrf8001_event_packet nrf8001_get_event();
#endif

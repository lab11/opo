#ifndef NRF8001_H
#define NRF8001_H

#include "contiki.h"
#include "nrf8001_setup.h"
#include "sys/clock.h"
#include "spi-arch.h"
#include "spi.h"
#include "dev/ssi.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/leds.h"
#include "dev/ioc.h"
#include <stdint.h>
#include <string.h>
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"

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
#define NRF8001_SET_LOCAL_DATA 	0x0D
#define NRF8001_SEND_DATA 		0x15
#define NRF8001_SEND_DATA_ACK 	0x16
#define NRF8001_REQUEST_DATA 	0x17
#define NRF8001_SEND_DATA_NACK 	0x18

// Data events
#define NRF8001_DATA_CREDIT_EVENT 	0x8A
#define NRF8001_DATA_ACK_EVENT 		0x8B
#define NRF8001_DATA_RECEIVED_EVENT 0x8C
#define NRF8001_PIPE_ERROR_EVENT 	0x8D

// Error checking
#define NRF8001_PARAMETER_BOUND_ERROR 0xFF

// ACI Status codes
#define ACI_STATUS_TRANSACTION_CONTINUE 0x01
#define ACI_STATUS_TRANSACTION_COMPLETE 0x02

// Connected packet breakdown
#define NRF8001_ADDRESS_TYPE 		0x00
#define NRF8001_PEER_ADDRESS_START 	0x01
#define NRF8001_PEER_ADDRESS_STOP 	0x06

// BlE connection address types
#define NRF8001_PUBLIC_ADDRESS 							0x01
#define NRF8001_RANDOM_STATIC_ADDRESS 					0x02
#define NRF8001_RANDOM_RESOLVABLE_PRIVATE_ADDRESS  		0x03
#define NRF8001_RANDOM_UNRESOLVABLE_PRIVATE_ADDRESS 	0x04

// ACI Error Codes
#define ACI_STATUS_ERROR_DATA_SIZE				0x8F
#define ACI_STATUS_ERROR_PIPE_INVALID			0x90
#define ACI_STATUS_ERROR_CREDIT_NOT_AVAILABLE	0x91
#define ACI_STATUS_ERROR_PEER_ATT_ERROR			0x92
#define ACI_STATUS_ERROR_PIPE_TYPE_INVALID      0x95
#define ACI_STATUS_ERROR_PIPE_STATE_INVALID		0x96
#define ACI_STATUS_ERROR_DEVICE_STATE_INVALID	0x83

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

typedef struct{
	uint32_t debug;
	uint8_t my_id;
	uint8_t handler; // id of the handler the message is sent from
	uint8_t status;
	uint32_t throwaway; // We use Opo's transmit functinality, which writes a delta timestamp here
} nrf8001_debug_t;

typedef void (* nrf8001_callback_t)(uint8_t event, uint8_t payload_length, uint8_t payload[30]);

void nrf8001_register_callback(uint8_t event, nrf8001_callback_t c);
void nrf8001_register_sleep_callback(void *callback); // Sleep command doesn't get an event response

void nrf8001_init();
void nrf8001_enable();
void nrf8001_reset();
void nrf8001_test(uint8_t test_type);
void nrf8001_echo(uint8_t packet_length, uint8_t *packet);
void nrf8001_setup();
void nrf8001_connect(uint16_t timeout, uint16_t advertising);
void nrf8001_disconnect(uint8_t reason);
void nrf8001_bond(uint16_t timeout, uint16_t advertising);
void nrf8001_sleep();
void nrf8001_wakeup();
void nrf8001_send_data_ack(uint8_t pipenum);
void nrf8001_change_timing_request(uint16_t interval_min,
								   uint16_t interval_max,
								   uint16_t slave_latency,
								   uint16_t timeout);
void nrf8001_send_data(uint8_t pipe, uint8_t packet_length, uint8_t *packet);
void nrf8001_open_remote_pipe(uint8_t pipe);


uint8_t nrf8001_check_rdyn();
void nrf8001_event_callback(uint8_t port, uint8_t pin);
void nrf8001_cmd_callback(uint8_t port, uint8_t pin);

#endif

#include "nrf8001.h"
#include "cpu.h"
#include "cc2538-rf.h"
#include <stdbool.h>
#include <stdio.h>
static nrf8001_command_packet nrf8001_cmd = {0};
static nrf8001_setup_msg_t setup_msgs[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;
static uint8_t setup_counter = 0;

static nrf8001_callback_t callbacks[15];
static uint8_t callback_tracker[15] = {0};
static void (*sleep_callback)();
static uint8_t callback_state = 0;
static uint8_t command_credit = 1; // Make sure the previous command has been processed before sending the next one
enum  nrf8001_state {
	IDLE,
	COMMAND,
	EVENT
} mstate = IDLE;

// Debug stuff
static uint8_t current_command = 0;
static bool meh = false;
static bool debug_clear = true;

PROCESS(nrf8001_cmd_process, "BleCmd");
PROCESS(nrf8001_event_process, "BleEvent");

static const uint8_t reverse_table[] = {
        0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
        0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
        0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
        0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
        0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
        0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
        0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
        0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
        0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
        0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
        0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
        0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
        0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
        0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
        0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
        0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
        0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
        0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
        0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
        0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
        0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
        0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
        0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
        0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
        0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
        0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
        0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
        0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
        0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
        0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
        0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
        0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
    };

static inline uint8_t reverse_bits(uint8_t b) {
    // Algorithm from Bit Twiddling hacks
    // https://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith32Bits
	uint8_t br = 0;
	br = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
	return br;
}

static void default_handler(uint8_t event, uint8_t payload_length, uint8_t payload[30]) {}
static void default_sleep_handler() {return;}

void nrf8001_register_callback(uint8_t event, nrf8001_callback_t c) {
	callbacks[event - 0x80] = c;
	callback_tracker[event - 0x80] = 1;
}

static inline void reverse_nrf8001_cmd() {
	int i = 0;
	for(i=0;i<nrf8001_cmd.length-1;i++) {
		nrf8001_cmd.packet[i] = reverse_table[nrf8001_cmd.packet[i]];
	}
	nrf8001_cmd.length = reverse_table[nrf8001_cmd.length];
	nrf8001_cmd.command = reverse_table[nrf8001_cmd.command];
}

static inline void REQN_SET() { SPI_CS_SET(NRF8001_REQN_PORT, NRF8001_REQN_PIN); }
static inline void REQN_CLR() { SPI_CS_CLR(NRF8001_REQN_PORT, NRF8001_REQN_PIN); }
static inline void RESET_SET() { GPIO_SET_PIN(NRF8001_RESET_PORT_BASE, NRF8001_RESET_PIN_MASK); }
static inline void RESET_CLR() { GPIO_CLR_PIN(NRF8001_RESET_PORT_BASE, NRF8001_RESET_PIN_MASK); }

/**
   Helper function to disable RDYN pin
*/
static inline void disable_and_clear_rdyn() {
	GPIO_DISABLE_INTERRUPT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
    GPIO_CLEAR_INTERRUPT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
    GPIO_DISABLE_POWER_UP_INTERRUPT(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN_MASK);
    GPIO_CLEAR_POWER_UP_INTERRUPT(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN_MASK);
}

static inline void enable_rdyn() {
	GPIO_ENABLE_INTERRUPT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_ENABLE_POWER_UP_INTERRUPT(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN_MASK);
}

void nrf8001_event_callback(uint8_t port, uint8_t pin) {
	disable_and_clear_rdyn();
	process_poll(&nrf8001_event_process);
}

PROCESS_THREAD(nrf8001_event_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		uint8_t rdyn_state = 1;
		rdyn_state = GPIO_READ_PIN(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
		nrf8001_event_packet nrf8001_ep = {0};
		INTERRUPTS_DISABLE();
			if(rdyn_state == 0) {
				uint8_t debug = 0;
				uint8_t i = 0;
				nrf8001_ep.length = 0;
				nrf8001_ep.event = 0;
				for(i=0;i<30;i++) { nrf8001_ep.packet[i] = 0; }
				spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
				SPI_FLUSH();
				REQN_CLR();
				SPI_READ(debug);
				SPI_READ(nrf8001_ep.length);
				SPI_READ(nrf8001_ep.event);

				nrf8001_ep.length = reverse_table[nrf8001_ep.length];
				if(nrf8001_ep.length > 0 && nrf8001_ep.length < 30) {
					for(i=0; i < nrf8001_ep.length-1; i++) { SPI_READ(nrf8001_ep.packet[i]); }
					nrf8001_ep.event = reverse_table[nrf8001_ep.event];
					for(i=0; i < nrf8001_ep.length-1;i++) { nrf8001_ep.packet[i] = reverse_table[nrf8001_ep.packet[i]]; }
				}
				else if(nrf8001_ep.length > 30) { leds_on(LEDS_RED); }
				REQN_SET();
				while(REG(SSI0_BASE + SSI_SR) & SSI_SR_BSY) {}
			}
		INTERRUPTS_ENABLE();
		if(rdyn_state == 0 && nrf8001_ep.length > 0 && nrf8001_ep.length < 30) {
			// Handle continuation cases for user. Right now only works fo setup
			if(nrf8001_ep.event == NRF8001_COMMAND_RESPONSE_EVENT) {
				uint8_t cmd_op_code = nrf8001_ep.packet[0];
				uint8_t cmd_status = nrf8001_ep.packet[1];
				if(cmd_status == ACI_STATUS_TRANSACTION_CONTINUE) {
					if(cmd_op_code == NRF8001_SETUP) { nrf8001_setup(); }
				}
				else if(cmd_status == ACI_STATUS_ERROR_DEVICE_STATE_INVALID) {}

			}
			else {
				if (nrf8001_ep.event >= 0x80) {
					uint8_t e = nrf8001_ep.event - 0x80;
					if(callback_tracker[e] == 1) {
						callbacks[e](nrf8001_ep.event, nrf8001_ep.length - 1, nrf8001_ep.packet);
					}
				}
			}
		}
		if(nrf8001_cmd.command != 0) {
			gpio_register_callback( (gpio_callback_t) nrf8001_cmd_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
			REQN_CLR();
		}
		enable_rdyn();
	}
	PROCESS_END();
}

void nrf8001_cmd_callback(uint8_t port, uint8_t pin) {
	current_command = 0;
    disable_and_clear_rdyn();
	process_poll(&nrf8001_cmd_process);
}

PROCESS_THREAD(nrf8001_cmd_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		INTERRUPTS_DISABLE();
		uint8_t rdyn_state = 1;
		int i = 0;
		rdyn_state = GPIO_READ_PIN(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
		if(rdyn_state == 0) {
			uint8_t plength = reverse_table[nrf8001_cmd.length] - 1;
			spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
			SPI_FLUSH();
			SPI_WRITE(nrf8001_cmd.length);
			SPI_WRITE(nrf8001_cmd.command);
			for(i=0;i < plength;i++) { SPI_WRITE(nrf8001_cmd.packet[i]); }
            callback_state = 0;
			gpio_register_callback( (gpio_callback_t) nrf8001_event_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
			REQN_SET();
			nrf8001_cmd.command = 0;
		}
		INTERRUPTS_ENABLE();
		if(reverse_table[nrf8001_cmd.command] == NRF8001_SLEEP && rdyn_state == 0) {
			for(i=0;i<2;i++) { clock_delay_usec(40000); }
			(*sleep_callback)();
		}
		else {
			if(reverse_table[nrf8001_cmd.command] == NRF8001_WAKEUP && rdyn_state == 0) {
				for(i=0;i<2;i++) { clock_delay_usec(40000); }
				GPIO_CLEAR_INTERRUPT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
				if(GPIO_READ_PIN(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK) == 0) {
					process_poll(&nrf8001_event_process);
				}
			}
		}
		enable_rdyn();
	}
	PROCESS_END();
}

static inline void set_command() {
	reverse_nrf8001_cmd();
    callback_state = 1;
	gpio_register_callback( (gpio_callback_t) nrf8001_cmd_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
	REQN_CLR();
}


// Function to configure RDYN pin for event callbacks
// parameters are ignored. only exist to meet SENSORS_SENSOR spec
static int config_event_callback(int type, int value) {
	GPIO_SOFTWARE_CONTROL(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_SET_INPUT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_DETECT_EDGE(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_DETECT_FALLING(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_POWER_UP_ON_FALLING(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN_MASK);
	ioc_set_over(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN, IOC_OVERRIDE_PUE);
	gpio_register_callback( (gpio_callback_t) nrf8001_event_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
	GPIO_ENABLE_POWER_UP_INTERRUPT(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN_MASK);
	GPIO_ENABLE_INTERRUPT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
    nvic_interrupt_enable(NRF8001_RDYN_PORT);
	return 0;
}

// Initialize nrf8001 hardware
void nrf8001_init() {
	uint8_t i = 0;
	spi_cs_init(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	GPIO_SET_INPUT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_SET_OUTPUT(NRF8001_REQN_PORT_BASE, NRF8001_REQN_PIN_MASK);
	GPIO_SOFTWARE_CONTROL(NRF8001_RESET_PORT_BASE, NRF8001_RESET_PIN_MASK);
	GPIO_SET_OUTPUT(NRF8001_RESET_PORT_BASE, NRF8001_RESET_PIN_MASK);
	GPIO_SET_PIN(NRF8001_RESET_PORT_BASE, NRF8001_RESET_PIN_MASK);
	ioc_set_over(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN, IOC_OVERRIDE_PUE);
	ioc_set_over(NRF8001_REQN_PORT, NRF8001_REQN_PIN, IOC_OVERRIDE_DIS);

	REQN_SET();

	for(i=0;i<15;i++) { callbacks[i] = (nrf8001_callback_t) default_handler; }
    sleep_callback = default_sleep_handler;
	process_start(&nrf8001_cmd_process, NULL);
	process_start(&nrf8001_event_process, NULL);
}

// Enable nrf8001 functionality.
void nrf8001_enable() {
	uint8_t rdyn_state = 1;
	config_event_callback(0,0); // Set up nrf8001 EVENT event callback
	rdyn_state = GPIO_READ_PIN(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	if (rdyn_state == 0) { nrf8001_event_callback(0,0); }
}

void nrf8001_reset() {
    RESET_CLR();
    clock_delay_usec(10);
    RESET_SET();
}

// Enter Direct Test Mode. See Bluetooth Core Spec v4.0, Vol 6, Part F
// Test type 0x01 = Enable DTM over UART
// Test type 0x02 = Enable DTM over ACI
// Test type 0x03 = Exit DTM
void nrf8001_test(uint8_t test_type) {
	nrf8001_cmd.length = 2;
	nrf8001_cmd.command = NRF8001_TEST;
	nrf8001_cmd.packet[0] = test_type;
	reverse_nrf8001_cmd();
	gpio_register_callback( (gpio_callback_t) nrf8001_cmd_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
	REQN_CLR();
}

// Echo back ACI (SPI) commands for testing purposes
// Must already be in correct test mode nrf8001_test(2)
// Max packet length of 30. Max 29 data bytes
void nrf8001_echo(uint8_t packet_length, uint8_t *packet) {
	int i = 0;
	nrf8001_cmd.length = packet_length + 1;
	nrf8001_cmd.command = NRF8001_ECHO;
	for(i=0;i<packet_length;i++) {
		nrf8001_cmd.packet[i] = packet[i];
	}
	reverse_nrf8001_cmd();
	gpio_register_callback( (gpio_callback_t) nrf8001_cmd_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
	REQN_CLR();
}

void nrf8001_setup() {
	int i = 0;
	nrf8001_cmd.length = setup_msgs[setup_counter].payload[0];
	nrf8001_cmd.command = NRF8001_SETUP;
	for(i=2;i < setup_msgs[setup_counter].payload[0]+1;i++) { nrf8001_cmd.packet[i-2] = setup_msgs[setup_counter].payload[i]; }
	reverse_nrf8001_cmd();
	if(setup_counter+1 < NB_SETUP_MESSAGES) { setup_counter++; }
	gpio_register_callback( (gpio_callback_t) nrf8001_cmd_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
	REQN_CLR();
}

void nrf8001_connect(uint16_t timeout, uint16_t ad_interval) {
	nrf8001_cmd.length = 5;
	nrf8001_cmd.command = NRF8001_CONNECT;
	nrf8001_cmd.packet[0] = timeout & 0x00ff;
	nrf8001_cmd.packet[1] = timeout >> 8;
	nrf8001_cmd.packet[2] = ad_interval & 0x00ff;
	nrf8001_cmd.packet[3] = ad_interval >> 8;
	current_command = NRF8001_CONNECT;
	reverse_nrf8001_cmd();
	meh = true;
	gpio_register_callback( (gpio_callback_t) nrf8001_cmd_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
	REQN_CLR();
}

void nrf8001_bond(uint16_t timeout, uint16_t ad_interval) {
	nrf8001_cmd.length = 5;
	nrf8001_cmd.command = NRF8001_BOND;
	nrf8001_cmd.packet[0] = timeout & 0x00ff;
	nrf8001_cmd.packet[1] = timeout >> 8;
	nrf8001_cmd.packet[2] = ad_interval & 0x00ff;
	nrf8001_cmd.packet[3] = ad_interval >> 8;
	reverse_nrf8001_cmd();
	gpio_register_callback( (gpio_callback_t) nrf8001_cmd_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
	REQN_CLR();
}

void nrf8001_sleep() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_SLEEP;
	set_command();
}

void nrf8001_wakeup() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_WAKEUP;
	current_command = NRF8001_WAKEUP;
	enable_rdyn();
	set_command();
}

void nrf8001_read_dynamice_data() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_READ_DYNAMIC_DATA;
	set_command();
}

void nrf8001_write_dynamic_data() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_WRITE_DYNAMIC_DATA;
	set_command();
}

void nrf8001_get_battery_level() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_GET_BATTERY_LEVEL;
	set_command();
}

void nrf8001_get_temperature() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_GET_TEMPERATURE;
	set_command();
}

void nrf8001_radio_reset() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_RADIO_RESET;
	set_command();
}


// Disconnect from peer device
// Reason == 0x01: Remote user terminated connection
// Reason == 0x02: Unacceptable connection timing
void nrf8001_disconnect(uint8_t reason) {
	nrf8001_cmd.length = 2;
	nrf8001_cmd.command = NRF8001_DISCONNECT;
	nrf8001_cmd.packet[0] = reason;
	set_command();
}

void nrf8001_set_tx_power(uint8_t tx_pwr) {
	nrf8001_cmd.length = 2;
	nrf8001_cmd.command = NRF8001_SET_TX_POWER;
	nrf8001_cmd.packet[0] = tx_pwr;
	set_command();
}

void nrf8001_change_timing_request(uint16_t interval_min,
								   uint16_t interval_max,
								   uint16_t slave_latency,
								   uint16_t timeout) {
	nrf8001_cmd.length = 9;
	nrf8001_cmd.command = NRF8001_CHANGE_TIMING_REQUEST;
	nrf8001_cmd.packet[0] = interval_min & 0x00ff;
	nrf8001_cmd.packet[1] = interval_min >> 8;
	nrf8001_cmd.packet[2] = interval_max & 0x00ff;
	nrf8001_cmd.packet[3] = interval_max >> 8;
	nrf8001_cmd.packet[4] = slave_latency & 0x00ff;
	nrf8001_cmd.packet[5] = slave_latency >> 8;
	nrf8001_cmd.packet[6] = timeout & 0x00ff;
	nrf8001_cmd.packet[7] = timeout >> 8;
	set_command();
}

void nrf8001_open_remote_pipe(uint8_t pipe) {
	nrf8001_cmd.length = 2;
	nrf8001_cmd.command = NRF8001_OPEN_REMOTE_PIPE;
	nrf8001_cmd.packet[0] = pipe;
	set_command();
}

void nrf8001_set_app_latency(uint8_t latency_enable, uint16_t latency) {
	nrf8001_cmd.length = 4;
	nrf8001_cmd.command = NRF8001_SET_APPLICATION_LATENCY;
	nrf8001_cmd.packet[0] = latency_enable;
	nrf8001_cmd.packet[1] = latency & 0x00ff;
	nrf8001_cmd.packet[2] = latency >> 8;
	set_command();
}

void nrf8001_set_key(uint8_t key[6]) {
	uint8_t i = 0;
	nrf8001_cmd.length = 8;
	nrf8001_cmd.command = NRF8001_SET_KEY;
	nrf8001_cmd.packet[0] = 0x01;
	for(i=0; i < 6;i++) { nrf8001_cmd.packet[i+1] = key[i]; }
	set_command();
}

void nrf8001_open_adv_pipe(uint8_t pipe[8]) {
	uint8_t i = 0;
	nrf8001_cmd.length = 9;
	nrf8001_cmd.command = NRF8001_OPEN_ADV_PIPE;
	for(i=0; i < 8;i++) { nrf8001_cmd.packet[i] = pipe[i]; }
	set_command();
}

void nrf8001_broadcast(uint16_t timeout, uint16_t adv_interval) {
	nrf8001_cmd.length = 5;
	nrf8001_cmd.command = NRF8001_BROADCAST;
	nrf8001_cmd.packet[0] = timeout & 0x00ff;
	nrf8001_cmd.packet[1] = timeout >> 8;
	nrf8001_cmd.packet[2] = adv_interval & 0x00ff;
	nrf8001_cmd.packet[3] = adv_interval >> 8;
	set_command();
}

void nrf8001_bond_security_request() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_BOND_SEC_REQUEST;
	set_command();
}

void nrf8001_directed_connect() {
	nrf8001_cmd.length = 1;
	nrf8001_cmd.command = NRF8001_DIRECTED_CONNECT;
	set_command();
}

void nrf8001_close_remote_pipe(uint8_t pipe) {
	nrf8001_cmd.length = 2;
	nrf8001_cmd.command = NRF8001_CLOSE_REMOTE_PIPE;
	nrf8001_cmd.packet[0] = pipe;
	set_command();
}

void nrf8001_set_local_data(uint8_t pipe, uint8_t packet_length, uint8_t *packet) {
	uint8_t i = 0;
	nrf8001_cmd.length = packet_length + 1;
	nrf8001_cmd.command = NRF8001_SET_LOCAL_DATA;
	nrf8001_cmd.packet[0] = pipe;
	for(i=0;i<packet_length;i++) { nrf8001_cmd.packet[i+1] = packet[i]; }
	set_command();
}

void nrf8001_send_data(uint8_t pipe, uint8_t packet_length, uint8_t *packet) {
	uint8_t i = 0;
	nrf8001_cmd.length = packet_length + 2;
	nrf8001_cmd.command = NRF8001_SEND_DATA;
	nrf8001_cmd.packet[0] = pipe;
	for(i=0;i<packet_length;i++) { nrf8001_cmd.packet[i+1] = packet[i]; }
	set_command();
}

void nrf8001_send_data_ack(uint8_t pipe) {
	nrf8001_cmd.length = 2;
	nrf8001_cmd.command = NRF8001_SEND_DATA_ACK;
	nrf8001_cmd.packet[0] = pipe;
	set_command();
}
void nrf8001_request_data(uint8_t pipe) {
	nrf8001_cmd.length = 2;
	nrf8001_cmd.command = NRF8001_REQUEST_DATA;
	nrf8001_cmd.packet[0] = pipe;
	set_command();
}

void nrf8001_send_data_nack(uint8_t pipe, uint8_t error_code) {
	nrf8001_cmd.length = 3;
	nrf8001_cmd.command = NRF8001_SEND_DATA_NACK;
	nrf8001_cmd.packet[0] = pipe;
	nrf8001_cmd.packet[1] = error_code;
	set_command();
}

void nrf8001_register_sleep_callback(void *callback) {
	sleep_callback = callback;
}

uint8_t nrf8001_check_rdyn() {
    return GPIO_READ_PIN(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
}

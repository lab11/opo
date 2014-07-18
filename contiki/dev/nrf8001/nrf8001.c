#include "contiki.h"
#include "nrf8001.h"
#include "spi-arch.h"
#include "spi.h"
#include "dev/ssi.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/leds.h"

#define NRF8001_RDYN_PORT_BASE GPIO_PORT_TO_BASE(NRF8001_RDYN_PORT)
#define NRF8001_RDYN_PIN_MASK GPIO_PIN_MASK(NRF8001_RDYN_PIN)


nrf8001_command_packet cmd;
nrf8001_event_packet ep;

// Algorithm from Bit Twiddling hacks
// https://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith32Bits
// Neccessary
static uint8_t reverse_bits(uint8_t b) {
	uint8_t br = 0;
	br = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
	return br;
}

static inline void REQN_SET() {
	SPI_CS_SET(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
}

static inline void REQN_CLR() {
	SPI_CS_CLR(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
}

static void nrf8001_event_callback() {
	uint8_t debug = 0;
	uint8_t i = 0;
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	REQN_CLR();
	SPI_READ(debug);
	SPI_READ(ep.length);
	ep.length = reverse_bits(ep.length);
	SPI_READ(ep.event);
	ep.event = reverse_bits(ep.event);
	for(i=0; i < ep.length; i++) {
		SPI_READ(ep.packet[i]);
	}
	REQN_SET();
	for(i=0; i<ep.length;i++) {
		ep.packet[i] = reverse_bits(ep.packet[i]);
	}
	sensors_changed(&nrf8001_event);
}

static void nrf8001_cmd_callback() {
	int i = 0;
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	SPI_WRITE(cmd.length);
	SPI_WRITE(cmd.command);
	for(i=0;i < cmd.length-1;i++) {
		SPI_WRITE(cmd.packet[i]);
	}
	REQN_SET();
}


// Function to configure RDYN pin for event callbacks
// parameters are ignored. only exist to meet SENSORS_SENSOR spec
static int config_event_callback(int type, int value) {
	GPIO_SOFTWARE_CONTROL(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_SET_INPUT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_DETECT_EDGE(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_DETECT_FALLING(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_ENABLE_INTERRUPT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	ioc_set_over(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN, IOC_OVERRIDE_PUE);
	nvic_interrupt_enable(NRF8001_RDYN_PORT);
	gpio_register_callback(nrf8001_event_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
}

// Initialize nrf8001 hardware
void nrf8001_init() {
	spi_cs_init(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	REQN_SET();
}

// Enable nrf8001 functionality.
void nrf8001_enable() {
	uint8_t rdyn_state = 1;
	config_event_callback(0,0);
	rdyn_state = GPIO_READ_PIN(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	if (rdyn_state == 0) {
			nrf8001_event_callback();
	}
}

// Enter Direct Test Mode. See Bluetooth Core Spec v4.0, Vol 6, Part F
// Test type 0x01 = Enable DTM over UART
// Test type 0x02 = Enable DTM over ACI
// Test type 0x03 = Exit DTM
void nrf8001_test(uint8_t test_type) {
	cmd.length = 2;
	cmd.command = TEST;
	cmd.packet[0] = test_type;
	gpio_register_callback(nrf8001_cmd_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);
	REQN_CLR();
}

// Echo back ACI (SPI) commands for testing purposes
// Must already be in correct test mode nrf8001_test(2)
// Max packet length of 30. Max 29 data bytes
void nrf8001_echo(uint8_t packet_length, uint8_t *packet) {
	int i = 0;
	REQN_CLR();
	SPI_WRITE(packet_length);
	SPI_WRITE(ECHO);
	for(i=0; i < packet_length; i++) {
		SPI_WRITE(packet[i]);
	}
	REQN_SET();
}

void spi_write_test() {
	spi_cs_init(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	spi_enable();
	SPI_CS_CLR(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
	SPI_WRITE(8);
	SPI_CS_SET(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
}

// Event accessor function
nrf8001_event_packet nrf8001_get_event() {
	return ep;
}

SENSORS(&nrf8001_event);
SENSORS_SENSOR(nrf8001_event, NRF8001_SENSOR, NULL, config_event_callback, NULL);
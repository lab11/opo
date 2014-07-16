#include "contiki.h"
#include "nrf8001.h"
#include "spi-arch.h"
#include "spi.h"
#include "dev/ssi.h"
#include "dev/gpio.h"
#include "dev/ioc.h"

#define NRF8001_RDYN_PORT_BASE GPIO_PORT_TO_BASE(NRF8001_RDYN_PORT)
#define NRF8001_RDYN_PIN_MASK GPIO_PIN_MASK(NRF8001_RDYN_PIN)


nrf8001_event_packet *ep;
nrf8001_command_packet cmd;

inline void REQN_SET() {
	SPI_CS_SET(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
}

inline void REQN_CLR() {
	SPI_CS_CLR(NRF8001_REQN_PORT, NRF8001_REQN_PIN);
}

static void nrf8001_event_callback() {
	uint8_t debug = 0;
	uint8_t i = 0;
	REQN_CLR();
	SPI_READ(debug);
	SPI_READ(ep->length);
	SPI_READ(ep->event);
	for(i=0; i < ep->length; i++) {
		SPI_READ(ep->packet[i]);
	}
	REQN_SET();
	sensors_changed(&nrf8001_event);
}

static void nrf8001_cmd_callback() {
	int i = 0;
	SPI_WRITE(cmd.length);
	SPI_WRITE(cmd.command);
	for(i=0;i < cmd.length-1;i++) {
		SPI_WRITE(cmd.packet[i]);
	}
	REQN_SET();
}



// Initialize nrf8001 hardware
void nrf8001_init(nrf8001_event_packet *p) {
	ep = p;
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 8);
	spi_cs_init(NRF8001_REQN_PORT, NRF8001_REQN_PIN);

    // Configure rdyn pin
	GPIO_SOFTWARE_CONTROL(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_SET_INPUT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_DETECT_EDGE(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_DETECT_FALLING(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	GPIO_ENABLE_INTERRUPT(NRF8001_RDYN_PORT_BASE, NRF8001_RDYN_PIN_MASK);
	ioc_set_over(NRF8001_RDYN_PORT, NRF8001_RDYN_PIN, IOC_OVERRIDE_PUE);
	nvic_interrupt_enable(NRF8001_RDYN_PORT);
	gpio_register_callback(nrf8001_event_callback, NRF8001_RDYN_PORT, NRF8001_RDYN_PIN);

	REQN_SET();
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

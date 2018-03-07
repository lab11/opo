#include "adxl362.h"
#include <stdint.h>
#include "spi-arch.h"
#include "spi.h"
#include "ssi.h"
#include "dev/ssi.h"
#include "clock.h"
#include "sst25vf.h"

static void (*user_int_two_callback) ();

static inline void accel_set_cs() {
	SPI_CS_SET(ADXL362_CS_PORT_NUM, ADXL362_CS_PIN_NUM);
}

static inline void accel_clear_cs() {
	SPI_CS_CLR(ADXL362_CS_PORT_NUM, ADXL362_CS_PIN_NUM);
}

static void int_two_callback(uint8_t port, uint8_t pin) {
	GPIO_CLEAR_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);
	user_int_two_callback();
}

void accel_write_register(uint8_t reg, uint8_t data) {
  accel_set_cs();
  accel_clear_cs();
  spi_write_byte(WRITE_ADXL362_REGISTER);
  spi_write_byte(reg);
  spi_write_byte(data);
  spi_flush_buffer();
  accel_set_cs();
}

uint8_t accel_read_register(uint8_t reg) {
  uint8_t mdata;
  accel_set_cs();
  accel_clear_cs();
  spi_flush_buffer();
  spi_write_byte(READ_ADXL362_REGISTER);
  spi_write_byte(reg);
  spi_flush_buffer();
  spi_read_byte(&mdata);
  spi_flush_buffer();
  accel_set_cs();
  return mdata;
}

uint8_t accel_get_status() {
	return accel_read_register(STATUS);
}

void accel_set_activity_threshold(uint16_t thresh) {
	if(thresh > 0xFF) {
		uint8_t lsb = thresh;
		uint8_t msb = thresh >> 8 && 0x07;
		accel_write_register(THRESH_ACT_L, lsb);
		accel_write_register(THRESH_ACT_H, msb);
	}
	else {
		uint8_t lsb = thresh;
		accel_write_register(THRESH_ACT_L, lsb);
	}
}

void accel_set_inactivity_threshold(uint16_t thresh) {
	if(thresh > 0xFF) {
		uint8_t lsb = thresh;
		uint8_t msb = thresh >> 8 && 0x07;
		accel_write_register(THRESH_INACT_L, lsb);
		accel_write_register(THRESH_INACT_H, msb);
	}
	else {
		uint8_t lsb = thresh;
		accel_write_register(THRESH_INACT_L, lsb);
	}
}

void accel_set_inactivity_timer(uint16_t t) {
	if(t > 0xFF) {
		uint8_t lsb = t;
		uint8_t msb = t >> 8;
		accel_write_register(TIME_INACT_L, lsb);
		accel_write_register(TIME_INACT_H, msb);
	}
	else {
		uint8_t lsb = t;
		accel_write_register(TIME_INACT_L, lsb);
	}
}

void accel_enable_loop_mode() {
	accel_write_register(ACT_INACT_CTL, 0x3F); // setup loop mode
	accel_write_register(POWER_CTL, 0x0E); // Enable loop mode
}

// Set up all the pins neccessary
void accel_init() {
	spi_cs_init(ADXL362_CS_PORT_NUM, ADXL362_CS_PIN_NUM);
}

// Set up int two pins and register a callback for it.
void accel_setup_int_two_awake(void (*u_int_two_callback) ()) {
	// Setup int two gpio pin interrupts
	GPIO_SOFTWARE_CONTROL(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_SET_INPUT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	ioc_set_over(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM, IOC_OVERRIDE_DIS);
	GPIO_DETECT_EDGE(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_DETECT_RISING(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_POWER_UP_ON_RISING(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_MASK);

	// Setup the interrupts
	nvic_interrupt_enable(ADXL362_INTTWO_NVIC);
	GPIO_CLEAR_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);
	GPIO_ENABLE_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_ENABLE_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);

	// Set up int two callback handling
	user_int_two_callback = u_int_two_callback;
	gpio_register_callback((gpio_callback_t) int_two_callback, ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);

	// Enable the interrupt
	accel_get_status(); // clear any pending interrupt.
	accel_write_register(INTMAP2, 0x40);
}

void accel_enable_int_two_awake() {
	// Setup int two gpio pin interrupts
	GPIO_SOFTWARE_CONTROL(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_SET_INPUT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	ioc_set_over(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM, IOC_OVERRIDE_DIS);
	GPIO_DETECT_EDGE(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_DETECT_RISING(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_POWER_UP_ON_RISING(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_MASK);

	// Setup the interrupts
	nvic_interrupt_enable(ADXL362_INTTWO_NVIC);
	GPIO_CLEAR_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);
	GPIO_ENABLE_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_ENABLE_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);

	// Enable the interrupt
	accel_get_status(); // clear any pending interrupt.
	accel_write_register(INTMAP2, 0x40); 
}

void accel_disable_int_two() {
	GPIO_CLEAR_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);
	GPIO_DISABLE_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_DISABLE_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);
	accel_get_status();
	accel_write_register(INTMAP2,0);
}

void accel_soft_reset() {
	accel_write_register(SOFT_RESET, 0x52);
	clock_delay_usec(10000); 
}

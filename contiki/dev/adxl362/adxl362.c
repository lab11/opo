#include "adxl362.h"
#include <stdint.h>
#include "spi-arch.h"
#include "spi.h"
#include "ssi.h"
#include "dev/ssi.h"

static uint8_t activity_control_register = 0;
static uint8_t power_control_register = 0;

static inline void accel_set_cs() {
	SPI_CS_SET(ADXL362_CS_PORT_NUM, ADXL362_CS_PIN_NUM);
}

static inline void accel_clear_cs() {
	SPI_CS_CLR(ADXL362_CS_PORT_NUM, ADXL362_CS_PIN_NUM);
}

void accel_write_register(uint8_t reg, uint8_t data) {
  accel_clear_cs();
  spi_write_byte(WRITE_ADXL362_REGISTER);
  spi_write_byte(reg);
  spi_write_byte(data);
	spi_flush_buffer();
  accel_set_cs();
}

uint8_t accel_read_register(uint8_t reg) {
  uint8_t mdata;
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

void accel_set_int_two(uint8_t i) {
	accel_write_register(INTMAP2, i);
}

void accel_set_thresh_time(uint8_t t) {}

void accel_enable_activity_monitor() {
  activity_control_register |= ACT_EN;
  accel_write_register(ACT_INACT_CTL, activity_control_register);
}

void accel_disable_activity_monitor() {
  activity_control_register &= ~ACT_EN;
  accel_write_register(ACT_INACT_CTL, activity_control_register);
}

void accel_enable_inactivity_monitor() {
  activity_control_register |= INACT_EN;
  accel_write_register(ACT_INACT_CTL, activity_control_register);
}

void accel_disable_inactivity_monitor() {
  activity_control_register &= ~INACT_EN;
  accel_write_register(ACT_INACT_CTL, activity_control_register);
}

void accel_set_activity_ref_abs() {
  activity_control_register &= ~ACT_REF;
  accel_write_register(ACT_INACT_CTL, activity_control_register);
}

void accel_set_activity_ref_rel() {
  activity_control_register |= ACT_REF;
  accel_write_register(ACT_INACT_CTL, activity_control_register);
}

void accel_set_inactivity_ref_abs() {
  activity_control_register &= ~INACT_REF;
  accel_write_register(ACT_INACT_CTL, activity_control_register);
}

void accel_set_inactivity_ref_rel() {
  activity_control_register |= INACT_REF;
  accel_write_register(ACT_INACT_CTL, activity_control_register);
}

void accel_set_measurement_mode() {
	power_control_register |= MEASUREMENT_MODE;
	accel_write_register(POWER_CTL, power_control_register);
}

void accel_set_standby_mode() {
	power_control_register &= ~STANDBY_MODE;
	accel_write_register(POWER_CTL, power_control_register);
}

void accel_enable_wakeup_mode() {
	power_control_register |= WAKEUP_MODE;
	accel_write_register(POWER_CTL, power_control_register);
}

void accel_disable_wakeup_mode() {
	power_control_register &= ~WAKEUP_MODE;
 	accel_write_register(POWER_CTL, power_control_register);
}

void accel_soft_reset() {
	accel_write_register(SOFT_RESET, 0x52); // 0x52 is the letter R in ASCII and UNICODE
}

void accel_set_normal_noise() {

}

void accel_set_low_noise() {

}

void accel_set_ultralow_noise() {

}

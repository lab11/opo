#pragma once

#include <stdint.h>

#define WRITE_ADXL362_REGISTER	0x0A
#define READ_ADXL362_REGISTER 0x0B
#define READ_ADXL362_FIFO 	0x0D

#define DEVID_AD 0x00
#define DEVID_MST 0x01
#define PARTID 0x02
#define REVID 0x03
#define XDATA 0x08
#define YDATA 0x09
#define ZDATA 0x0A
#define STATUS 0x0B
#define FIFO_ENTRIES_L 0x0C
#define FIFO_ENTRIES_H 0x0D
#define XDATA_L 0x0E
#define XDATA_H 0x0F
#define YDATA_L 0x10
#define YDATA_H 0x11
#define ZDATA_L 0x12
#define ZDATA_H 0x13
#define TEMP_L 0x14
#define TEMP_H 0x15
#define SOFT_RESET 0x1F
#define THRESH_ACT_L 0x20
#define THRESH_ACT_H 0x21
#define TIME_ACT 0x22
#define THRESH_INACT_L 0x23
#define THRESH_INACT_H 0x24
#define TIME_INACT_L 0x25
#define TIME_INACT_H 0x26
#define ACT_INACT_CTL 0x27
#define FIFO_CONTROL 0x28
#define FIFO_SAMPLES 0x29
#define INTMAP1 0x2A
#define INTMAP2 0x2B
#define FILTER_CTL 0x2C
#define POWER_CTL 0x2D
#define SELF_TEST 0x2E

// Bit mask for activity control reg
#define ACT_EN 0x01
#define ACT_REF  0x02
#define INACT_EN  0x04
#define INACT_REF  0x06
#define LINK_LOOP  0x30

// Link loop bit masks
#define LINK_LOOP_DEFAULT  0x00
#define LINK_LOOP_LINKED  0x10
#define LINK_LOOP_LOOP  0x30

//Bit masks for power control
#define WAKEUP_MODE 0x04
#define MEASUREMENT_MODE  0x02
#define STANDBY_MODE  0xFC

uint8_t accel_get_status();
void accel_set_activity_threshold(uint16_t thresh);
void accel_set_inactivity_threshold(uint16_t thresh);
void accel_set_inactivity_timer(uint16_t t); 
void accel_setup_int_two_awake(void (*u_int_two_callback) ());
void accel_enable_int_two_awake(); 
void accel_disable_int_two();
void accel_enable_loop_mode();
void accel_init();
void accel_soft_reset();
void accel_write_register(uint8_t reg, uint8_t data);
uint8_t accel_read_register(uint8_t reg);

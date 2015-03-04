#include <stdint.h>
#include "dev/i2c.h"
#include <stdint.h>
#include <time.h>

#ifndef RV4162_H
#define RV4162_H

#define hS          0x00   // Tenhs and 100ths of seconds
#define SEC         0x01
#define MIN         0x02
#define HR          0x03
#define DAY         0x04
#define DATE        0x05
#define MONTH       0x06
#define YEAR        0x07
#define CAL         0x08
#define WATCH       0x09
#define ALMONTH     0x0A
#define ALDATE      0x0B
#define ALMIN       0x0C
#define ALSEC       0x0E
#define RV4162_ADDR 0x68

#define RV4162_BCD_TO_BINARY(v) ((v & 0x0F) + (((v & 0x10)>>4)*10) + (((v & 0x20)>>5)*20) + (((v & 0x40)>>6)*40))

void rv4162_read_full_time(uint8_t *full_time); // Read the time
time_t get_unixtime();
void set_unixtime(time_t time);

void rv4162_set_time(uint8_t *full_time); // Set the time

void rv4162_disable_clkout();

void rv4162_init();

#endif
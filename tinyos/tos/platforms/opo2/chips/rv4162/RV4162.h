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

typedef enum {
  RV_IDLE,
  RV_WRITE_FULL_TIME_START_ADDRESS,
  RV_READ_FULL_TIME,
  RV_FULL_TIME_READ,
  RV_WRITE_ST_BIT,
  RV_WRITE_ST_BIT_2,
  RV_ST_BIT_WRITTEN,
  RV_SET_TIME,
  RV_TIME_SET,
  RV_RESET_TIME,
  RV_TIME_RESET
}i2c_state_t;

#endif
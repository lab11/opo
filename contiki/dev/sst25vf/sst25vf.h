#include "lib/sensors.h"
#ifndef SST25VF_H

#define SST25VF_H
#define READ                  0x03
#define FAST_READ_DUAL_IO     0xbb
#define FAST_READ_DUAL_OUTPUT 0x3b
#define HIGH_SPEED_READ       0x0b
#define SECTOR_ERASE          0x20
#define SMALL_BLOCK_ERASE     0x52
#define LARGE_BLOCK_ERASE     0xD8
#define CHIP_ERASE            0x60
#define CHIP_ERASE_2          0xc7
#define PAGE_PROGRAM          0x02
#define DUAL_IN_PAGE_PROGRAM  0xa2
#define RDSR                  0x05
#define EWSR                  0x50
#define WRSR                  0x01
#define WREN                  0x06
#define WRDI                  0x04
#define RDID                  0x90
#define RDID_2                0xab
#define JEDEC_ID              0x9f
#define EHLD                  0xaa
#define READ_SID              0x88
#define PROGRAM_SID           0xa5
#define LOCKOUT_SID           0x85
#define T_PROGRAM_SID         5
#define T_PAGE_PROGRAM        3
#define T_CHIP_ERASE		  50
#define T_BLOCK_ERASE         25
#define T_SECTOR_ERASE		  25

extern const struct sensors_sensor sst25vf_event;



void sst25vf_set_program_callback(void *callback);

void sst25vf_set_chip_erase_callback(void *callback);

void sst25vf_set_program_sid_callback(void *callback);

void sst25vf_turn_on();

void sst25vf_turn_off();

void sst25vf_read_page(uint32_t addr, void *rxBuffer, uint32_t rx_len);

void sst25vf_read_sid(uint8_t addr, void *rxBuffer, uint8_t rx_len);

void sst25vf_program_sid(uint8_t addr, void *txBuffer, uint8_t tx_len);

uint8_t sst25vf_read_status_register();

void sst25vf_write_enable();

void sst25vf_write_disable();

void sst25vf_program(uint32_t addr,
				     void *txBuffer,
					 uint32_t tx_len);

void sst25vf_chip_erase();

void sst25vf_ewsr();

void sst25vf_wrsr(uint8_t status_data);

void sst25vf_init();

void sst25vf_4kb_erase(uint32_t addr);
void sst25vf_32kb_erase(uint32_t addr);
void sst25vf_64kb_erase(uint32_t addr);

#endif
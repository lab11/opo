#ifndef HPL_AT45DB_H
#define HPL_AT45DB_H

#define BUFFER_WRITE_1                 0x84
#define BUFFER_WRITE_2                 0x87
#define BUFFER_TO_MEM_ERASE_1          0x83
#define BUFFER_TO_MEM_ERASE_2          0x86
#define CHIP_ERASE                     0xc7
#define CONTINUOUS_ARRAY_READ_LOW_FREQ 0x03
#define MEMORY_PAGE_READ               0xd2
#define DEEP_POWER_DOWN                0xb9
#define RESUME_FROM_PD                 0xab

#define PAGE_ERASE_AND_PROGRAM_TIME_MS  40
#define DEEP_POWER_DOWN_US              3
#define STANDY_US                       30

#endif
#ifndef OPO_2_FLASH_H
#define OPO_2_FLASH_H

#define CC2420_DEF_CHANNEL 16
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

typedef nx_struct opo_flash_msg {
    nx_uint8_t sid[8];
    nx_uint8_t readBuffer[8];
    nx_uint8_t statusRegister;
} opo_flash_t;

enum {
    OPO_FLASH_TEST = 27
};

#endif
#ifndef OPO_FLASH_H
#define OPO_FLASH_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

#define AMP3_ADC      Port60
#define SFDPIN        Port26

typedef nx_struct opo_flash_store_msg {
    nx_uint16_t tx_id;
    nx_uint16_t seq;
} oflash_msg_t;

typedef nx_struct opo_flash_store_base_msg {
    nx_uint16_t tx_id;
    nx_uint16_t ultrasonic_rf_dt;
    nx_uint8_t  full_time[8];
    nx_uint16_t seq;
    nx_uint16_t rx_fails;
    nx_uint16_t enable_rx_fails;
    nx_uint16_t tx_fails;
} oflash_base_msg_t;

typedef nx_struct id_store {
    uint16_t id;
    uint32_t seed;
} id_store_t

enum {
    OPO_FLASH_STORE = 30,
    OPO_FLASH_STORE_BASE = 40
};

#endif
#ifndef OPO_FLASH_READER_H
#define OPO_FLASH_READER_H

#define CC2420_DEF_CHANNEL 24

typedef nx_struct id_store {
    nx_uint16_t id;
    nx_uint32_t seed;
} id_store_t;

typedef nx_struct opo_flash_store_base_msg {
    nx_uint16_t tx_id;
    nx_uint16_t ultrasonic_rf_dt;
    nx_int8_t   rssi;
    nx_uint8_t  full_time[5];
    nx_uint32_t tx_seq;
    nx_uint16_t rx_fails;
} oflash_base_msg_t;

typedef nx_struct opo_flash_store_base_rf_msg {
    nx_uint16_t tx_id;
    nx_uint16_t rx_id;
    nx_uint16_t ultrasonic_rf_dt;
    nx_int8_t   rssi;
    nx_uint8_t  full_time[5];
    nx_uint32_t tx_seq;
    nx_uint16_t rx_fails;
} oflash_base_rf_msg_t;

enum {
    OPO_FLASH_READER = 32,
    OPO_FLASH_READER_BASE = 42
};

#endif
#ifndef OPO2_FLASH_H
#define OPO2_FLASH_H

#define CC2420_DEF_CHANNEL 23
#define OPO_CHANNEL 23
#define BASE_CHANNEL 16

#define AMP3_ADC      Port60
#define SFDPIN        Port26

typedef nx_struct opo_flash_store_msg {
    nx_uint32_t seq;
    nx_uint32_t reset_counter;
    nx_uint16_t last_tx_id;
    nx_uint8_t  m_full_time[8];
    nx_uint16_t dt_ul_rf;
    nx_uint8_t  last_full_time[8];
} oflash_msg_t;

typedef nx_struct opo_flash_store_base_msg {
    nx_uint16_t tx_id;
    nx_uint16_t ultrasonic_rf_dt;
    nx_int8_t   rssi;
    nx_uint8_t  full_time[8];
    nx_uint8_t  tx_full_time[8];
    nx_uint32_t m_reset_counter;
    nx_uint32_t tx_reset_counter;
    nx_uint32_t tx_seq;
    nx_uint32_t rx_fails;
} oflash_base_msg_t;

typedef nx_struct opo_flash_store_base_rf_msg {
    nx_uint16_t tx_id;
    nx_uint16_t ultrasonic_rf_dt;
    nx_int8_t   rssi;
    nx_uint8_t  full_time[8];
    nx_uint8_t  tx_full_time[8];
    nx_uint32_t m_reset_counter;
    nx_uint32_t tx_reset_counter;
    nx_uint32_t tx_seq;
    nx_uint32_t rx_fails;
} oflash_base_rf_msg_t;

typedef nx_struct id_store {
    nx_uint16_t id;
    nx_uint32_t seed;
} id_store_t;

typedef nx_struct opo_config {
    nx_uint32_t reset_counter;
    nx_uint8_t  valid;
} oconfig_t;

enum {
    OPO_FLASH_STORE = 30,
    OPO_FLASH_STORE_BASE = 40
};

#endif
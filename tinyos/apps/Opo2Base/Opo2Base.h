#ifndef OPOBASE_2_H
#define OPOBASE_2_H

#define CC2420_DEF_CHANNEL 16
#define CC2520_DEF_CHANNEL 16

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

typedef nx_struct opo_flash_2_gatd_msg {
    nx_uint16_t rx_id;
    nx_uint16_t tx_id;
    nx_uint16_t base_id;
    nx_uint16_t ultrasonic_rf_dt;
    nx_int8_t   rssi;
    nx_uint8_t  full_time[8];
    nx_uint8_t  tx_full_time[8];
    nx_uint32_t m_reset_counter;
    nx_uint32_t tx_reset_counter;
    nx_uint32_t tx_seq;
    nx_uint32_t rx_fails;
    nx_uint32_t version;
} oflash2_gatd_msg_t;

typedef nx_struct opo_flash_store_check_message {
    nx_uint16_t m_id;
} oflash_base_check_msg_t;

enum {
    OPO_FLASH_STORE_BASE = 11,
    OPO_FLASH_STORE_BASE_CHECK = 12,
    OPO_FLASH_STORE_BASE_CHECK_RECEIVE = 13
};

#endif

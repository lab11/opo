#ifndef OPO2_SPY_H
#define OPO2_SPY_H

#define CC2520_DEF_CHANNEL 23


typedef nx_struct opo_flash_store_msg {
    nx_uint32_t seq;
    nx_uint32_t reset_counter;
    nx_uint16_t last_tx_id;
    nx_uint8_t  m_full_time[8];
    nx_uint16_t dt_ul_rf;
    nx_uint8_t  last_full_time[8];
} oflash_msg_t;

typedef nx_struct opo_flash_gatd_store_msg {
    nx_uint32_t seq;
    nx_uint32_t reset_counter;
    nx_uint16_t m_id;
    nx_uint16_t last_tx_id;
    nx_uint8_t  m_full_time[8];
    nx_uint16_t dt_ul_rf;
    nx_uint8_t  last_full_time[8];
} oflash_gatd_msg_t;

enum {
    OPO_FLASH_STORE = 10
};

#endif

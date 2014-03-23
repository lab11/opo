#ifndef OPOBASE_H
#define OPOBASE_H

#define CC2420_DEF_CHANNEL 15
#define CC2520_DEF_CHANNEL 15


typedef nx_struct opo_flash_store_msg {
    nx_uint16_t tx_id;
    nx_uint32_t seq;
    nx_uint16_t last_tx_id;
    nx_uint16_t dt_ul_rf;
    nx_uint8_t  last_full_time[5];
    nx_uint32_t last_seq;
} oflash_msg_t;

enum {
    OPO_FLASH_STORE = 31
};

#endif

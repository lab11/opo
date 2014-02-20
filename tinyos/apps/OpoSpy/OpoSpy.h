#ifndef OPOBASE_H
#define OPOBASE_H

#define CC2420_DEF_CHANNEL 15
#define CC2520_DEF_CHANNEL 15


typedef nx_struct opo_flash_store_msg {
    nx_uint16_t tx_id;
    nx_uint16_t seq;
    nx_uint8_t full_time[5];
    nx_uint8_t buffer_index;
    nx_uint16_t last_tx_id;
    nx_uint16_t t_rf;
    nx_uint16_t t_ultrasonic_wake;
    nx_uint16_t t_ultrasonic_wake_falling;
    nx_uint16_t t_ultrasonic;
    nx_uint16_t t_ultrasonic_falling;
} oflash_msg_t;

enum {
    OPO_FLASH_STORE = 30
};

#endif

#ifndef OPO_VISUAL_STORE_H
#define OPO_VISUAL_STORE_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

#define AMP3_ADC      Port60
#define SFDPIN        Port26

#define MAX_NODES     40

//#define OPO_DEBUG

typedef nx_struct opo_visual_rf_msg {
    nx_uint8_t  tx_id[6];
    nx_uint_32_t tx_count;
} ovis_msg_t;

typedef nx_struct opo_visual_base_msg {
    nx_uint8_t  rx_id[6];
    nx_uint8_t  tx_id[6];
    nx_uint16_t t_rf;
    nx_uint16_t t_ultrasonic;
    nx_uint8_t sec;
    nx_uint8_t min;
    nx_uint8_t h;
    nx_uint_32_t tx_count;
} ovis_base_msg_t;

enum {
    OPO_VISUAL_STORE_DEMO = 23,
    OPO_VISUAL_STORE_BASE = 23,
    PREP = 5
};

#endif
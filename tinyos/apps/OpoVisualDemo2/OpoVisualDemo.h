#ifndef OPO_VISUAL_H
#define OPO_VISUAL_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

#define AMP3_ADC      Port60
#define SFDPIN        Port26

#define MAX_NODES     40

//#define OPO_DEBUG

typedef nx_struct opo_visual_rf_msg {
    nx_uint8_t  tx_id[6];
    nx_uint16_t seq;
} ovis_msg_t;

typedef nx_struct opo_visual_base_msg {
    nx_uint8_t  rx_id[6];
    nx_uint8_t  tx_id[6];
    nx_uint16_t ultrasonic_rf_dt;
    nx_uint16_t ultrasonic_wake_dt;
    nx_uint16_t ultrasonic_dt;
    nx_uint16_t full_time;
    nx_uint16_t seq;
    nx_uint16_t rx_fails;
    nx_uint16_t tx_fails;
} ovis_base_msg_t;

enum {
    OPO_VISUAL_DEMO = 12,
    OPO_VISUAL_BASE = 22,
    PREP = 5
};

#endif
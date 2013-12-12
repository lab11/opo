#ifndef OPO_TX_H
#define OPO_TX_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

#define AMP3_ADC      Port60
#define SFDPIN        Port26

#define MAX_NODES     40

//#define OPO_DEBUG

typedef nx_struct opo_blink_rf_msg {
    nx_uint8_t  m_id[6];
} oblink_msg_t;

typedef nx_struct opo_blink_base_msg {
    nx_uint8_t  m_id[6];
    nx_uint8_t  o_id[6];
    nx_uint32_t range;
} oblink_base_msg_t;


enum {
    OPO_BLINK = 11,
    OPO_BLINK_BASE = 21,
    PREP = 5
};

#endif
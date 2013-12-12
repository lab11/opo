#ifndef OPO_RX_H
#define OPO_RX_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

#define AMP3_ADC      Port60
#define SFDPIN        Port26

typedef nx_struct opo_range_msg {
    nx_uint32_t sequence;
    nx_uint8_t  m_id[6];
} opo_rf_msg_t;

enum {
    OPO = 17,
    BASE = 20,
    PREP = 5
};

#endif
#ifndef OPO_RX_H
#define OPO_RX_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

#define AMP3_ADC      Port60
#define SFDPIN        Port26

typedef nx_struct opo_rf_msg {
    nx_uint32_t test;
} opo_rf_msg_t;

enum {
    OPO_RF_TEST = 26
};

#endif
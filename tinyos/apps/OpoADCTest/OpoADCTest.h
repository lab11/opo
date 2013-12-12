#ifndef OPO_ADC_H
#define OPO_ADC_H

#define CC2420_DEF_CHANNEL 15
#define NODE_CHANNEL 15
#define BASE_CHANNEL 16

#define M_SAMPLES 120

typedef nx_struct opo_adc_msg {
    nx_uint16_t readings[6];
    nx_uint16_t times[6];
    nx_uint16_t rf_time;
} opo_adc_msg_t;

enum {
    OPO_ADC_BASE = 24,
    OPO_RX_TX = 13
};

#endif
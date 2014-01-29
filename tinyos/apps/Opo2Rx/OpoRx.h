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

typedef nx_struct opo_rx_base_msg {
    nx_uint8_t  rx_id[6];
    nx_uint16_t t_rf;
    nx_uint16_t t_ultrasonic_wake;
    nx_uint16_t t_ultrasonic_wake_falling;
    nx_uint16_t t_ultrasonic;
    nx_uint16_t t_ultrasonic_falling;
} opo_rx_base_msg_t;

enum {
    OPO_RX_TX = 13,
    OPO_RX_BASE = 23
};

#endif
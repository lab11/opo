#ifndef OPO_RX_H
#define OPO_RX_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

#define AMP3_ADC      Port60
#define SFDPIN        Port26

typedef nx_struct opo_range_msg {
    nx_uint32_t sequence;
    nx_uint8_t  m_id[8];
} opo_rf_msg_t;

typedef nx_struct opo_rx_base_msg {
    nx_uint8_t  rx_id[8];
    nx_uint8_t  tx_id[8];
    nx_uint16_t rx_t_rf;
    nx_uint16_t rx_t_ultrasonic;
    nx_uint32_t tx_seq;
    nx_uint16_t rx_seq;
    nx_int8_t   rx_rssi;
    nx_uint8_t  rx_fail_count;
} opo_rx_base_msg_t;

enum {
    OPO_RX_TX = 13,
    OPO_RX_BASE = 23
};

#endif
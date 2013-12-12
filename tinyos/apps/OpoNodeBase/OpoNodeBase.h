#ifndef OPONODEBASE_H
#define OPONODEBASE_H

#define CC2420_DEF_CHANNEL 16
#define CC2520_DEF_CHANNEL 16

typedef nx_struct opo_rec_msg {
    nx_uint8_t valid; //validates data read from flash
    nx_uint32_t range; // Range is given in micrometers to avoid decimals
    nx_uint32_t sequence; // Used to match nodes to the same broadcast
    nx_uint8_t o_id[6]; // ID from transmitter's (other node's) ds2411
    nx_int8_t rssi;
    nx_uint8_t tx_pwr;
    nx_uint8_t seconds;
    nx_uint8_t minutes;
    nx_uint8_t hours;
} opo_rec_t;

typedef nx_struct opo_base_msg {
    opo_rec_t rec;
    nx_uint8_t m_id[6];
} opo_bmsg_t;

typedef nx_struct opo_blink_base_msg {
    nx_uint8_t  m_id[6];
    nx_uint8_t  o_id[6];
    nx_uint32_t range;
} oblink_base_msg_t;

typedef nx_struct opo_visual_base_msg {
    nx_uint8_t  rx_id[6];
    nx_uint8_t  tx_id[6];
    nx_uint32_t range;
    nx_uint8_t sec;
    nx_uint8_t min;
    nx_uint8_t h;
} ovis_base_msg_t;

typedef nx_struct opo_rx_base_msg {
    nx_uint8_t  rx_id[6];
    nx_uint32_t range;
} opo_rx_base_msg_t;

enum {
    OPO_DATA = 20,
    OPO_BLINK = 21,
    OPO_VISUAL_BASE = 22,
    OPO_RX_BASE = 23
};

#endif

#ifndef OPO_SIMPLE_DEMO_H
#define OPO_SIMPLE_DEMO_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

#define SFD_INT_PIN        Port26

#define NUM_NODES 10

typedef nx_struct opo_simple_rf_msg {
    nx_uint8_t  m_id[6];
} opo_simple_rf_msg_t;

enum {
    OPO_SIMPLE = 18,
    PREP = 5
};

#endif
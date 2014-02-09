#ifndef OPO_ID_WRITER_H
#define OPO_ID_WRITER_H

#define CC2420_DEF_CHANNEL 16

typedef nx_struct id_store {
    nx_uint16_t id;
    nx_uint32_t seed;
} id_store_t;

enum {
    OPO_ID_WRITER = 31,
    OPO_ID_WRITER_BASE = 41
};

#endif
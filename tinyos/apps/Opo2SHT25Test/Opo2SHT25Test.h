#ifndef OPO_2_SHT_H

#define OPO_2_SHT_H
#define CC2420_DEF_CHANNEL 16
#define OPO_SHT25_TEST     28

typedef nx_struct opo_sht25_test_message {
	nx_uint16_t temp;
	nx_uint16_t rh;
} opo_sht25_test_msg_t;

#endif
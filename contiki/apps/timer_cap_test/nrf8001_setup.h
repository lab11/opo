/**
* This file is autogenerated by nRFgo Studio 1.17.0.3211
*/

#ifndef SETUP_MESSAGES_H__
#define SETUP_MESSAGES_H__

#define SETUP_ID 1
#define SETUP_FORMAT 3 /** nRF8001 D */
#define ACI_DYNAMIC_DATA_SIZE 132

/* Service: GATT - Characteristic: Service Changed - Pipe: TX_ACK */
#define PIPE_GATT_SERVICE_CHANGED_TX_ACK          1
#define PIPE_GATT_SERVICE_CHANGED_TX_ACK_MAX_SIZE 4

/* Service: OpoBLETest - Characteristic: OpoTest - Pipe: TX */
#define PIPE_OPOBLETEST_OPOTEST_TX          2
#define PIPE_OPOBLETEST_OPOTEST_TX_MAX_SIZE 6

/* Service: OpoBLETest - Characteristic: OpoTest - Pipe: TX_ACK */
#define PIPE_OPOBLETEST_OPOTEST_TX_ACK          3
#define PIPE_OPOBLETEST_OPOTEST_TX_ACK_MAX_SIZE 6

/* Service: OpoBLETest - Characteristic: OpoTest - Pipe: RX_ACK */
#define PIPE_OPOBLETEST_OPOTEST_RX_ACK          4
#define PIPE_OPOBLETEST_OPOTEST_RX_ACK_MAX_SIZE 6

/* Service: OpoBLETest - Characteristic: OpoTest - Pipe: SET */
#define PIPE_OPOBLETEST_OPOTEST_SET          5
#define PIPE_OPOBLETEST_OPOTEST_SET_MAX_SIZE 6


#define NUMBER_OF_PIPES 5

#define SERVICES_PIPE_TYPE_MAPPING_CONTENT {\
  {ACI_STORE_LOCAL, ACI_TX_ACK},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX_ACK},   \
  {ACI_STORE_LOCAL, ACI_RX_ACK},   \
  {ACI_STORE_LOCAL, ACI_SET},   \
}

#define GAP_PPCP_MAX_CONN_INT 0xffff /**< Maximum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_MIN_CONN_INT  0xffff /**< Minimum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_SLAVE_LATENCY 2
#define GAP_PPCP_CONN_TIMEOUT 0xffff /** Connection Supervision timeout multiplier as a multiple of 10msec, 0xFFFF means no specific value requested */

#define NRF8001_MAX_SETUP_MESSAGES 20
#define NRF8001_SETUP_MESSAGES_CONTENT {\
    {0x00,\
        {\
            0x07,0x06,0x00,0x00,0x03,0x02,0x42,0x07,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x05,0x01,0x01,0x00,0x00,0x06,0x00,0x01,\
            0xc2,0x0a,0x18,0x1c,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x03,0x90,0x00,0xff,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x38,0xff,0xff,0x02,0x58,0x0a,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,\
            0x00,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x05,0x06,0x10,0x54,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x00,0x04,0x04,0x02,0x02,0x00,0x01,0x28,0x00,0x01,0x00,0x18,0x04,0x04,0x05,0x05,0x00,\
            0x02,0x28,0x03,0x01,0x02,0x03,0x00,0x00,0x2a,0x04,0x04,0x14,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x1c,0x05,0x00,0x03,0x2a,0x00,0x01,0x4f,0x70,0x6f,0x56,0x33,0x63,0x73,0x65,0x6d,0x69,\
            0x2e,0x63,0x6f,0x6d,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x38,0x05,0x05,0x00,0x04,0x28,0x03,0x01,0x02,0x05,0x00,0x01,0x2a,0x06,0x04,0x03,0x02,\
            0x00,0x05,0x2a,0x01,0x01,0x47,0x01,0x04,0x04,0x05,0x05,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x54,0x06,0x28,0x03,0x01,0x02,0x07,0x00,0x04,0x2a,0x06,0x04,0x09,0x08,0x00,0x07,0x2a,\
            0x04,0x01,0xff,0xff,0xff,0xff,0x02,0x00,0xff,0xff,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x70,0x02,0x02,0x00,0x08,0x28,0x00,0x01,0x01,0x18,0x04,0x04,0x05,0x05,0x00,0x09,0x28,\
            0x03,0x01,0x22,0x0a,0x00,0x05,0x2a,0x26,0x04,0x05,0x04,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x8c,0x0a,0x2a,0x05,0x01,0x00,0x00,0x00,0x00,0x46,0x14,0x03,0x02,0x00,0x0b,0x29,0x02,\
            0x01,0x00,0x00,0x04,0x04,0x02,0x02,0x00,0x0c,0x28,0x00,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xa8,0x0a,0x18,0x04,0x04,0x05,0x05,0x00,0x0d,0x28,0x03,0x01,0x02,0x0e,0x00,0x24,0x2a,\
            0x06,0x04,0x09,0x08,0x00,0x0e,0x2a,0x24,0x01,0x6e,0x52,0x46,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xc4,0x78,0x78,0x78,0x78,0x78,0x06,0x04,0x08,0x07,0x00,0x0f,0x29,0x04,0x01,0x19,0x00,\
            0x00,0x00,0x01,0x00,0x00,0x04,0x04,0x05,0x05,0x00,0x10,0x28,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xe0,0x03,0x01,0x02,0x11,0x00,0x25,0x2a,0x06,0x04,0x05,0x04,0x00,0x11,0x2a,0x25,0x01,\
            0x31,0x35,0x38,0x37,0x06,0x04,0x08,0x07,0x00,0x12,0x29,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xfc,0x01,0x19,0x00,0x00,0x00,0x01,0x00,0x00,0x04,0x04,0x02,0x02,0x00,0x13,0x28,0x00,\
            0x01,0x1c,0x18,0x04,0x04,0x05,0x05,0x00,0x14,0x28,0x03,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x18,0x3e,0x15,0x00,0x66,0x66,0x76,0x14,0x07,0x06,0x00,0x15,0x66,0x66,0x01,0x4f,0x50,\
            0x4f,0x57,0x49,0x4e,0x06,0x04,0x08,0x07,0x00,0x16,0x29,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x17,0x06,0x21,0x34,0x01,0x19,0x00,0x00,0x00,0x01,0x00,0x00,0x46,0x14,0x03,0x02,0x00,0x17,0x29,0x02,\
            0x01,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x17,0x06,0x40,0x00,0x2a,0x05,0x01,0x00,0x04,0x04,0x00,0x0a,0x00,0x0b,0x66,0x66,0x01,0x00,0x96,0x04,\
            0x00,0x15,0x00,0x17,\
        },\
    },\
    {0x00,\
        {\
            0x09,0x06,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x06,0x06,0xf0,0x00,0x03,0x51,0xa1,\
        },\
    },\
}

#endif

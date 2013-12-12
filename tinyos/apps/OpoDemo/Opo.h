#ifndef OPO_H
#define OPO_H

#define CC2420_DEF_CHANNEL 15
#define OPO_CHANNEL 15
#define BASE_CHANNEL 16

/* SFD stuff */
#define	SFDPIN			Port26 //Alternative line to generate SFD interrupts
#define SFDLATCH		Port52 //Soft latch for the sfd going high
#define SFDCAPPIN		Port41

/* Gate Control ports */
#define TXRXSEL			Port50 // Controls the SPDT switch
#define TXGATE			Port53 // This powergates the transmit side

/* Ultrasonic Rx Stuff */
#define UCAPPIN         Port43
#define AMP3_ADC        Port60

#define SFDCAPTURE      CaptureB1
#define UCAPTURE        CaptureB3
#define SFDCAPCONT      ControlB1
#define UCAPCONT        ControlB3

/* Misc */
#define AMP3_ADC        Port60
#define leds2gpio       Port57 //debug

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

typedef nx_struct oppo_base_msg {
    opo_rec_t rec;
    nx_uint8_t m_id[6];
} opo_bmsg_t;

typedef nx_struct opo_range_msg {
    nx_uint8_t m_id[6]; // ID from transmitter's (this node's) ds2411
    nx_uint32_t sequence; // A node unique identifier of our current broadcast
    nx_uint8_t tx_pwr;
} opo_rf_msg_t;

enum {
	AM_URANGE = 17,
    OPO_PROBE = 18,
    OPO_ACK = 19,
    OPO_DATA = 20
};

#endif

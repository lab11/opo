#ifndef OPO_H
#define OPO_H

/* SFD stuff */
#define SFD_LATCH                Port52 // Soft latch for the sfd going high
#define SFD_PIN                  Port41 // SFD Pin on EPIC

/* Gate Control ports */
#define TXRX_SEL                 Port50 // Controls the SPDT switch
#define TX_GATE                  Port53 // This powergates the transmit side

/* Ultrasonic Rx Stuff */
#define U_CAP_PIN                Port43

#define SFD_CAPTURE              CaptureB1
#define U_CAPTURE                CaptureB3
#define SFD_CAPTURE_CONTROL      ControlB1
#define U_CAPTURE_CONTROL        ControlB3

//#define OPO_DEBUG

#endif

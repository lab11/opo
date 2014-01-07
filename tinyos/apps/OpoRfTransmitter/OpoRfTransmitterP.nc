#include "OpoRfTransmitter.h"

module OpoRfTransmitterP {
    uses {
        interface Boot;
        interface Leds;
        interface AMSend;
        interface SplitControl;
        interface Timer<TMilli> as RfSendTimer;
    }
}

implementation {
    int i = 0;
    message_t packet;
    opo_rf_msg_t *p;

    event void Boot.booted() {
        p = (opo_rf_msg_t*) call AMSend.getPayload(&packet, sizeof(opo_rf_msg_t));
        p->test = 1;
        call SplitControl.start();
    }

    event void AMSend.sendDone(message_t *msg, error_t err) {
        call Leds.led1Toggle();
        call RfSendTimer.startOneShot(5000);
    }

    event void RfSendTimer.fired() {
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_rf_msg_t));
    }

    event void SplitControl.startDone(error_t err) {
        call Leds.led0On();
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_rf_msg_t));
    }

    event void SplitControl.stopDone(error_t err) {}
}
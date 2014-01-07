#include "OpoRfReceiver.h"

module OpoRfReceiverP {
    uses {
        interface Boot;
        interface Leds;
        interface Receive;
        interface SplitControl;
    }
}

implementation {
    int i = 0;

    event void Boot.booted() {
        call SplitControl.start();
    }

    event message_t* Receive.receive(message_t *msg, void *payload, uint8_t len) {
        opo_rf_msg_t *mData = (opo_rf_msg_t *) payload;
        if(mData->test == 1) {
            call Leds.led1Toggle();
        }

        return msg;
    }

    event void SplitControl.startDone(error_t err) {
        call Leds.led0On();
    }

    event void SplitControl.stopDone(error_t err) {}
}
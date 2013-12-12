#include "printf.h"
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
        printf("Booted\n");
        printfflush();
        call SplitControl.start();
    }

    event message_t* Receive.receive(message_t *msg, void *payload, uint8_t len) {

        opo_rf_msg_t *mData = (opo_rf_msg_t *) payload;
        call Leds.led0Toggle();
        printf("seq: %lu\n", mData->sequence);
        printf("id: ");
        for(i = 0; i < 6; i++) {
            printf("%u", mData->m_id[i]);
        }
        printf("\n");
        printfflush();

        return msg;
    }

    event void SplitControl.startDone(error_t err) {
        call Leds.led1Toggle();
    }

    event void SplitControl.stopDone(error_t err) {}
}
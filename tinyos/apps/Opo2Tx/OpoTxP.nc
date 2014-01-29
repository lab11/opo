#include "OpoTx.h"

module OpoTxP {
    uses {
        interface Boot;
        interface Leds;
        interface AMSend as PrepSend;
        interface Opo;
        interface Timer<TMilli> as OpoTxTimer;
        interface HplSST25VF064 as Flash;
    }
}

implementation {
    message_t packet;
    uint32_t range_sequence;
    uint8_t m_id[8] = {0};

    opo_rf_msg_t *p;
    int i;

    event void Boot.booted() {
        call Opo.setup_pins();
        call Flash.turnOn();
    }

    event void OpoTxTimer.fired() {
        call Leds.led0Toggle();
        call Leds.led1Toggle();

        p = (opo_rf_msg_t*) call PrepSend.getPayload(&packet,
                                                     sizeof(opo_rf_msg_t));
        p -> sequence = range_sequence;
        range_sequence++;

        call Opo.transmit(&packet, sizeof(opo_rf_msg_t));
    }

    event void PrepSend.sendDone(message_t* bufPtr, error_t error) {}


    event void Opo.transmit_done() {
        call OpoTxTimer.startOneShot(5000);
    }



    event void Opo.receive(uint16_t t_rf,
                           uint16_t t_ultrasonic_wake,
                           uint16_t t_ultrasonic_wake_falling,
                           uint16_t t_ultrasonic,
                           uint16_t t_ultrasonic_falling,
                           message_t* msg) {}
    event void Opo.receive_failed() {}
    event void Opo.transmit_failed() {}
    event void Opo.enable_receive_failed() {}


    event void Flash.turnedOn() {
        uint8_t addr[3] = {0,0,0};
        call Flash.read_sid(addr, &m_id[0], 8);
    }

    event void Flash.turnedOff() {
        call OpoTxTimer.startOneShot(1000);
    }

    event void Flash.read_sid_done(uint8_t *rxBuffer, uint16_t rx_len) {
        p = (opo_rf_msg_t*) call PrepSend.getPayload(&packet,
                                                     sizeof(opo_rf_msg_t));
        for(i = 0; i < 8; i++) {
            p -> m_id[i] = m_id[i];
        }
        call Flash.turnOff();
    }

    event void Flash.read_done(uint8_t *rxBuffer, uint16_t rx_len) {}
    event void Flash.chip_erase_done() {}
    event void Flash.page_program_done(uint8_t *txBuffer, uint16_t len) {}

}
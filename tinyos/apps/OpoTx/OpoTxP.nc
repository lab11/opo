#include "OpoTx.h"

module OpoTxP {
    uses {
        interface Boot;
        interface Leds;
        interface ReadId48 as IdReader;
        interface SplitControl as At45dbPower;
        interface AMSend as PrepSend;
        interface Opo;
        interface HplMsp430GeneralIO as AMP3_ADC;
        interface HplMsp430GeneralIO as SFDIntGpio;
        interface Timer<TMilli> as TxTimer;
    }
}

implementation {
    message_t packet;
    uint32_t range_sequence;
    uint32_t tx_interval_min = 0;
    uint8_t m_id[6];

    event void Boot.booted() {
        int i;
        opo_rf_msg_t *p;
        call IdReader.read(&m_id[0]);
        //call SFDIntGpio.makeInput();
        p = (opo_rf_msg_t*) call PrepSend.getPayload(&packet,
                                                     sizeof(opo_rf_msg_t));
        for(i = 0; i < 6; i++) {
            p -> m_id[i] = m_id[i];
        }

        call At45dbPower.stop();
        call AMP3_ADC.makeInput();

        call Opo.setup_pins();
        call TxTimer.startOneShot(2000);
    }

    event void TxTimer.fired() {
        int i;
        opo_rf_msg_t *p;
        p = (opo_rf_msg_t*) call PrepSend.getPayload(&packet,
                                                     sizeof(opo_rf_msg_t));
        p -> sequence = range_sequence;
        range_sequence++;

        call Leds.led0On();
        call Opo.transmit(&packet, sizeof(opo_rf_msg_t));
    }

    event void Opo.transmit_done() {
        call Leds.led0Off();
        call TxTimer.startOneShot(10000);
    }

    event void PrepSend.sendDone(message_t* bufPtr, error_t error) {}

    event void Opo.receive(uint16_t t_rf,
                           uint16_t t_ultrasonic_wake,
                           uint16_t t_ultrasonic_wake_falling,
                           uint16_t t_ultrasonic,
                           uint16_t t_ultrasonic_falling,
                           message_t* msg) {}
    event void Opo.receive_failed() {}
    event void Opo.transmit_failed() {}
    event void Opo.enable_receive_failed() {}

    event void At45dbPower.startDone(error_t err) {}
    event void At45dbPower.stopDone(error_t err) {}
}
#include "OpoTx.h"

module OpoTxP {
    uses {
        interface Boot;
        interface Leds;
        interface AMSend as PrepSend;
        interface Opo;
        interface Timer<TMilli> as OpoTxTimer;
        interface HplRV4162 as RTCHpl;
        interface HplMsp430GeneralIO as I2CSwitch;
    }
}

implementation {
    message_t packet;
    uint32_t range_sequence;
    uint8_t m_id[8] = {0};
    uint8_t initial_time[8] = {0};

    oflash_msg_t *p;
    int i;

    event void Boot.booted() {
        call Opo.setup_pins();
        initial_time[0] = 0;
        initial_time[1] = M_SEC;
        initial_time[2] = M_MIN;
        initial_time[3] = M_HOUR;
        initial_time[4] = M_WEEKDAY;
        initial_time[5] = M_DATE;
        initial_time[6] = M_MONTH;
        initial_time[7] = M_YEAR;
        call I2CSwitch.set();
        call RTCHpl.setTime(initial_time);
    }

    event void OpoTxTimer.fired() {
        call Leds.led0Toggle();
        call Leds.led1Toggle();

        p = (oflash_msg_t*) call PrepSend.getPayload(&packet,
                                                     sizeof(oflash_msg_t));
        p->seq = range_sequence;
        p->reset_counter = 0;
        p->last_tx_id = 0;
        p->dt_ul_rf = 0;
        range_sequence++;

        call I2CSwitch.set();
        call RTCHpl.readFullTime();
    }

    event void PrepSend.sendDone(message_t* bufPtr, error_t error) {}


    event void Opo.transmit_done() {
        call OpoTxTimer.startOneShot(3000);
    }

    event void RTCHpl.readFullTimeDone(error_t err, uint8_t *fullTime) {
        call I2CSwitch.clr();
        for(i=0;i<8;i++) {
            p->m_full_time[i] = fullTime[i];
        }
        call Opo.transmit(&packet, sizeof(oflash_msg_t));
    }

    event void RTCHpl.setTimeDone(error_t err) {
        call I2CSwitch.clr();
        call OpoTxTimer.startOneShot(500);
    }


    event void Opo.receive(uint16_t t_rf,
                           uint16_t t_ultrasonic,
                           message_t* msg) {}
    event void Opo.receive_failed(uint8_t rx_status) {}
    event void Opo.transmit_failed() {}
    event void Opo.enable_receive_failed() {}

}
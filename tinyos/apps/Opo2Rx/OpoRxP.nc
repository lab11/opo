#include "OpoRx.h"

module OpoRxP {
    uses {
        interface Boot;
        interface Leds;
        interface Random;
        interface ReadId48 as IdReader;
        interface SplitControl as At45dbPower;
        interface SplitControl as RfControl;
        interface CC2420Config;
        interface Packet;
        interface Opo;
        interface Timer<TMilli> as RxTimer;
        interface Timer<TMilli> as BaseTimer;
        interface AMSend as BaseSend;
    }
}

implementation {
    enum {RANGE, BASE} base = RANGE;
    message_t packet;
    message_t base_packet;
    uint8_t   fcount = 0; // receive failed count
    uint8_t   m_id[6];
    uint8_t   i; // used for for loops
    uint16_t m_t_rf;
    uint16_t m_t_ultrasonic_wake;
    uint16_t m_t_ultrasonic_wake_falling;
    uint16_t m_t_ultrasonic;
    uint16_t m_t_ultrasonic_falling;
    uint32_t guard;

    event void Boot.booted() {
        call IdReader.read(&m_id[0]);
        call At45dbPower.stop();
        call Opo.setup_pins();
        call Opo.enable_receive();
    }

    event void Opo.receive(uint16_t t_rf,
                           uint16_t t_ultrasonic_wake,
                           uint16_t t_ultrasonic_wake_falling,
                           uint16_t t_ultrasonic,
                           uint16_t t_ultrasonic_falling,
                           message_t* msg) {
        m_t_rf = t_rf;
        m_t_ultrasonic_wake = t_ultrasonic_wake;
        m_t_ultrasonic_wake_falling = t_ultrasonic_wake_falling;
        m_t_ultrasonic = t_ultrasonic;
        m_t_ultrasonic_falling = t_ultrasonic_falling;

        call Leds.led0Toggle();
        guard = call Random.rand32() % 50;
        call BaseTimer.startOneShot(100 + guard);
    }

    event void Opo.receive_failed() {
        fcount += 1;
        call RxTimer.startOneShot(50);
    }

    event void RxTimer.fired() {
        call Opo.enable_receive();
    }

    event void BaseTimer.fired() {
        opo_rx_base_msg_t *p;
        p = (opo_rx_base_msg_t*) call BaseSend.getPayload(&base_packet,
                                                          sizeof(opo_rx_base_msg_t));

        for(i = 0; i < 6; i++) {
            p->rx_id[i] = m_id[i];
        }
        p->t_rf = m_t_rf;
        p->t_ultrasonic_wake = m_t_ultrasonic_wake;
        p->t_ultrasonic_wake_falling = m_t_ultrasonic_wake_falling;
        p->t_ultrasonic = m_t_ultrasonic;
        p->t_ultrasonic_falling = m_t_ultrasonic_falling;

        base = BASE;
        call CC2420Config.setChannel(BASE_CHANNEL);
        call RfControl.start();
    }

    event void RfControl.startDone(error_t err) {
        if(base == BASE) {
            call BaseSend.send(AM_BROADCAST_ADDR, &base_packet, sizeof(opo_rx_base_msg_t));
        }
    }

    event void RfControl.stopDone(error_t err) {
        if(base == BASE) {
            base = RANGE;
            call CC2420Config.setChannel(OPO_CHANNEL);
            call RxTimer.startOneShot(50);
        }
    }

    event void BaseSend.sendDone(message_t *msg, error_t err) {
        call RfControl.stop();
    }

    event void Opo.transmit_done() {}
    event void Opo.transmit_failed() {}
    event void Opo.enable_receive_failed() {}

    event void CC2420Config.syncDone(error_t error) {}

    event void At45dbPower.startDone(error_t err) {}
    event void At45dbPower.stopDone(error_t err) {}

}
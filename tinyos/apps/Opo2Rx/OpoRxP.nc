#include "OpoRx.h"

module OpoRxP {
    uses {
        interface Boot;
        interface Leds;
        interface Random;
        interface SplitControl as RfControl;
        interface CC2420Config;
        interface CC2420Packet;
        interface Packet;
        interface Opo;
        interface Timer<TMilli> as RxTimer;
        interface Timer<TMilli> as BaseTimer;
        interface AMSend as BaseSend;
        interface HplSST25VF064 as Flash;
    }
}

implementation {
    enum {RANGE, BASE} base = RANGE;
    message_t packet;
    message_t base_packet;
    uint8_t   fcount = 0; // receive failed count
    uint8_t   m_id[8];
    uint8_t   i; // used for for loops
    uint16_t m_rx_seq = 0;
    uint32_t seq = 0;
    uint32_t guard;
    opo_rx_base_msg_t test;
    opo_rx_base_msg_t *p;

    event void Boot.booted() {
        uint8_t *testBuffer = (uint8_t *)&test;
        p = (opo_rx_base_msg_t*) call BaseSend.getPayload(&base_packet,
                                                          sizeof(opo_rx_base_msg_t));
        call Opo.setup_pins();
        call Flash.turnOn();
    }

    event void Opo.receive(uint16_t t_rf,
                           uint16_t t_ultrasonic_wake,
                           uint16_t t_ultrasonic_wake_falling,
                           uint16_t t_ultrasonic,
                           uint16_t t_ultrasonic_falling,
                           message_t* msg) {
        opo_rf_msg_t *txp;
        txp = (opo_rf_msg_t*) call Packet.getPayload(msg, sizeof(opo_rf_msg_t));
        m_rx_seq += 1;

        for(i = 0; i < 8; i++) {
            p->tx_id[i] = txp->m_id[i];
            p->rx_id[i] = m_id[i];
        }
        p->rx_t_rf = t_rf;
        p->rx_t_ultrasonic = t_ultrasonic;
        p->rx_rssi = call CC2420Packet.getRssi(msg);
        p->tx_seq = txp->sequence;
        p->rx_fail_count = fcount;
        p->rx_seq = m_rx_seq;

        call Leds.led0Toggle();

        guard = call Random.rand32() % 50;
        call BaseTimer.startOneShot(100 + guard);
    }

    event void Opo.receive_failed(uint8_t rx_status) {
        fcount += 1;
        call Leds.led1Toggle();
        call RxTimer.startOneShot(50);
    }

    event void RxTimer.fired() {
        call Opo.enable_receive();
    }

    event void BaseTimer.fired() {
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

    event void Flash.turnedOn() {
        uint8_t addr[3] = {0,0,0};
        call Flash.read_sid(addr, &m_id[0], 8);
    }

    event void Flash.turnedOff() {
        call Opo.enable_receive();
    }

    event void Flash.read_sid_done(uint8_t *rxBuffer, uint16_t rx_len) {
        call Flash.turnOff();
    }

    event void Flash.read_done(uint8_t *rxBuffer, uint16_t rx_len) {
    }

    event void Flash.chip_erase_done() {
    }

    event void Flash.page_program_done(uint8_t *txBuffer, uint16_t len) {
    }

    event void Opo.transmit_done() {}
    event void Opo.transmit_failed() {}
    event void Opo.enable_receive_failed() {}

    event void CC2420Config.syncDone(error_t error) {}

}
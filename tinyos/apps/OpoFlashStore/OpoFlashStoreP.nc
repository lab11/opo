#include "OpoFlashStore.h"

module OpoFlashStoreP {
    uses {
        interface Boot;
        interface Leds;
        interface RandomMt;
        interface SplitControl as FlashPower;
        interface Packet;
        interface Opo;
        interface Timer<TMilli> as TxTimer;
        interface Timer<TMilli> as RxTimer;
        interface PacketAcknowledgements as PacketAcks;
        interface HplRV4162;
        interface CC2420Config;
        interface BlockWrite;
    }
}

implementation {
    uint32_t RX_DELAY = 70;
    uint8_t i = 0;

    // RF packet setup
    message_t packet;
    oflash_msg_t *opo_data;

    // Timer delays
    uint32_t guard = 0;
    uint32_t dt = 0;
    uint32_t t0 = 0;
    uint32_t tn = 0;
    uint32_t rt = 0;

    // Opo Bookkeeping
    uint16_t rx_fails = 0;
    uint16_t tx_fails = 0;
    uint16_t seq = 0;

    // Initial time for rtc
    uint16_t initial_time[8];

    // node id and seed for rng
    uint16_t m_id = 0;
    uint32_t m_seed = 0;

    void setGuardTime();
    void getRemainingTimerTime();

    event void Boot.booted() {
        call Opo.setup_pins();
        call PacketAcks.noAck(&packet);
        RandomMt.seed(m_seed);

        setGuardTime();

        opo_data = (oflash_msg_t*) call Packet.getPayload(&packet,
                                                          sizeof(oflash_msg_t));
        call HplRV4162.setTime(initial_time);
    }

    event void RxTimer.fired() {
        call Opo.enable_receive();
    }

    event void TxTimer.fired() {
        p = (ovis_msg_t*) call Packet.getPayload(&packet,
                                                 sizeof(ovis_msg_t));
        p->seq = seq;

        call Opo.transmit(&packet, sizeof(ovis_msg_t));
    }

    event void Opo.transmit_done() {
        call RxTimer.stop();
        setGuardTime();
        seq += 1;
        call TxTimer.startOneShot(1000 + guard);
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void Opo.transmit_failed() {
        tx_fails += 1;
        call TxTimer.startOneShot(guard + 75);
    }

    event void Opo.receive(uint16_t t_rf,
                           uint16_t t_ultrasonic_wake,
                           uint16_t t_ultrasonic_wake_falling,
                           uint16_t t_ultrasonic,
                           uint16_t t_ultrasonic_falling,
                           message_t* msg) {

        uint16_t ultrasonic_wake_dt = 0;
        uint16_t ultrasonic_dt = 0;
        uint16_t ultrasonic_rf_dt = 0;

        call TxTimer.stop();
        tn = call TxTimer.getNow();
        t0 = call TxTimer.gett0();
        dt = call TxTimer.getdt();
        getRemainingTimerTime();

        opo_data = call Packet.getPayload(msg, sizeof(ovis_msg_t));
        call HplRV4162.readFullTime();
    }

    event void Opo.receive_failed() {
        rx_fails += 1;
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void Opo.enable_receive_failed() {
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void HplRV4162.readFullTimeDone(error_t err, uint8_t *fullTime) {
        // Store full time, re-enable Opo Transmit nad REceive
    }

    event void HplRV4162.setTimeDone(error_t err) {
        // After setting time, start Opo Protocol
        call RxTimer.startOneShot(RX_DELAY);
        call TxTimer.startOneShot(2000 + guard);
    }

    event void FlashPower.startDone(error_t err) {}

    event void BlockWrite.writeDone(storage_addr_t addr,
                                    void *buf,
                                    storage_len_t len,
                                    error_t error) {
        // Flush data to disk
        call BlockWrite.sync();
    }
    event void BlockWrite.syncDone(error_t err) {
        call At45dbPowerC.stop();
    }
    event void BlockWrite.eraseDone(error_t err) {}

    event void FlashPower.stopDone(error_t err) {
        // restart Opo protocol
        call RxTimer.startOneShot(RX_DELAY);
        call TxTimer.startOneShot(2000 + guard);
    }

    event void HplRV4162.writeSTBitDone(error_t err) {}
    event void HplRV4162.resetTimeDone(error_t err) {}



    event void CC2420Config.syncDone(error_t error) {}

    inline void setGuardTime() {
        guard = call Random.rand32();
        guard = guard % 2000;
    }

    inline void getRemainingTimerTime() {
        if (tn > t0) {
            if (dt > (tn - t0)) {
                rt = dt - (tn - t0);
            }
            else {
                rt = dt;
            }
        }
        else {
            rt = dt;
        }
    }

}
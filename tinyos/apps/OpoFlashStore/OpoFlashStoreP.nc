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
        interface CC2420Packet;
        interface BlockRead;
        interface BlockWrite;
    }
}

implementation {
    uint32_t RX_DELAY = 70;
    uint8_t i = 0;

    // RF packet setup
    message_t packet;
    oflash_msg_t *opo_data;
    oflash_msg_t *opo_rx_data;

    // Timer delays
    uint32_t guard = 0;
    uint32_t dt = 0;
    uint32_t t0 = 0;
    uint32_t tn = 0;
    uint32_t rt = 0;

    // Opo Bookkeeping
    uint16_t rx_fails = 0;
    uint16_t enable_rx_fails = 0;
    uint16_t tx_fails = 0;
    uint16_t seq = 0;

    // Initial time for rtc
    uint8_t initial_time[8];

    // Flash Storage Stuff
    oflash_base_msg_t buffer[12];
    uint8_t buffer_index = 0;
    uint16_t write_count = 0;
    storage_addr_t flash_addr = sizeof(id_store_t);

    // id and seed
    id_store_t m_id_store;

    void setGuardTime();
    void getRemainingTimerTime();

    event void Boot.booted() {
        call Opo.setup_pins();
        call PacketAcks.noAck(&packet);
        opo_data = (oflash_msg_t*) call Packet.getPayload(&packet,
                                                          sizeof(oflash_msg_t));
        call HplRV4162.setTime(initial_time);
    }

    event void RxTimer.fired() {
        call Opo.enable_receive();
    }

    event void TxTimer.fired() {
        opo_data->seq = seq;
        call Opo.transmit(&packet, sizeof(oflash_msg_t));
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

        call TxTimer.stop();
        tn = call TxTimer.getNow();
        t0 = call TxTimer.gett0();
        dt = call TxTimer.getdt();
        getRemainingTimerTime();

        opo_rx_data = call Packet.getPayload(msg, sizeof(oflash_msg_t));

        if (t_ultrasonic > t_rf) {
            buffer[buffer_index].ultrasonic_rf_dt = t_ultrasonic - t_rf;
            buffer[buffer_index].seq = opo_rx_data->seq;
            buffer[buffer_index].tx_id = opo_rx_data->tx_id;
            buffer[buffer_index].rx_fails = rx_fails;
            buffer[buffer_index].enable_rx_fails = enable_rx_fails;
            buffer[buffer_index].tx_fails = tx_fails;
            buffer[buffer_index].rssi = call CC2420Packet.getRssi(msg);
            call HplRV4162.readFullTime();
        }
        else {
            call RxTimer.startOneShot(RX_DELAY);
            call TxTimer.startOneShot(rt);
        }

    }

    event void Opo.receive_failed() {
        rx_fails += 1;
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void Opo.enable_receive_failed() {
        enable_rx_fails += 1;
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void HplRV4162.readFullTimeDone(error_t err, uint8_t *fullTime) {
        // read time and store to buffer;
        for(i=0; i<8; i++) {
            buffer[buffer_index].full_time[i] = fullTime[i];
        }

        if(write_count == 0) {
            if(buffer_index == 10) {
                buffer_index = 0;
                call BlockWrite.write(flash_addr, &buffer, sizeof(oflash_base_msg_t) * 11);
            }
            else{
                buffer_index++;
            }
        }
        else {
            if(buffer_index == 11) {
                buffer_index = 0;
                call BlockWrite.write(flash_addr, &buffer, sizeof(oflash_base_msg_t) * 12);
            }
            else {
                buffer_index++;
            }
        }
    }

    event void HplRV4162.setTimeDone(error_t err) {
        // After setting time, read out id
        call BlockRead.read(flash_addr, &m_id_store, sizeof(id_store_t));
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
        call FlashPower.stop();
    }
    event void BlockWrite.eraseDone(error_t err) {}

    event void BlockRead.readDone(storage_addr_t addr,
                                                    void *buf,
                                                    storage_len_t len,
                                                    error_t error) {
        call RandomMt.seed(m_id_store.seed);
        opo_data->tx_id = m_id_store.id;
        addr += sizeof(id_store_t);
        call FlashPower.stop();
    }

    event void FlashPower.stopDone(error_t err) {
        // Start Opo protocol
        write_count += 1;
        flash_addr = write_count * 256;
        setGuardTime();
        call RxTimer.startOneShot(RX_DELAY);
        call TxTimer.startOneShot(2000 + guard);
    }

    event void BlockRead.computeCrcDone(storage_addr_t addr,
                                                                storage_len_t len,
                                                                uint16_t crc,
                                                                error_t error) {}
    event void HplRV4162.writeSTBitDone(error_t err) {}
    event void HplRV4162.resetTimeDone(error_t err) {}
    event void CC2420Config.syncDone(error_t error) {}

    inline void setGuardTime() {
        guard = call RandomMt.rand32();
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
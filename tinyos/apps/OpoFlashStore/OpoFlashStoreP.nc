#include "OpoFlashStore.h"

module OpoFlashStoreP {
    uses {
        interface Boot;
        interface Leds;
        interface RandomMt;
        interface Packet;
        interface Opo;
        interface Timer<TMilli> as TxTimer;
        interface Timer<TMilli> as RxTimer;
        interface Timer<TMilli> as LedTimer;
        interface PacketAcknowledgements as PacketAcks;
        interface HplRV4162;
        interface CC2420Config;
        interface CC2420Packet;
        interface HplAt45db;
    }
}

implementation {
    #define BUFFER_SIZE 40

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
    uint32_t seq = 0;

    // Initial time for rtc
    uint8_t initial_time[8] = {0};

    // Flash Storage Stuff
    uint8_t buffer_index = 0;
    uint8_t max_buffer_index = 32;
    uint16_t page_count = 1;
    uint16_t writesize = 528;
    oflash_base_msg_t buffer[BUFFER_SIZE];

    // id and seed
    id_store_t m_id_store;

    void setGuardTime();
    void getRemainingTimerTime();

    event void Boot.booted() {
        m_id_store.seed = 0;
        m_id_store.id = 0;
        initial_time[0] = 0;
        initial_time[1] = M_SEC;
        initial_time[2] = M_MIN;
        initial_time[3] = M_HOUR;
        initial_time[4] = M_WEEKDAY;
        initial_time[5] = M_DATE;
        initial_time[6] = M_MONTH;
        initial_time[7] = M_YEAR;
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
            buffer[buffer_index].tx_seq = opo_rx_data->seq;
            buffer[buffer_index].tx_id = opo_rx_data->tx_id;
            buffer[buffer_index].rx_fails = rx_fails;
            buffer[buffer_index].rssi = call CC2420Packet.getRssi(msg);
            opo_data->last_tx_id = opo_rx_data->tx_id;
            opo_data->last_seq = opo_rx_data->seq;
            opo_data->dt_ul_rf = t_ultrasonic - t_rf;
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
        buffer[buffer_index].full_time[0] = fullTime[1];
        buffer[buffer_index].full_time[1] = fullTime[2];
        buffer[buffer_index].full_time[2] = fullTime[3];
        buffer[buffer_index].full_time[3] = fullTime[5];
        buffer[buffer_index].full_time[4] = fullTime[6];

        opo_data->last_full_time[0] = fullTime[1];
        opo_data->last_full_time[1] = fullTime[2];
        opo_data->last_full_time[2] = fullTime[3];
        opo_data->last_full_time[3] = fullTime[5];
        opo_data->last_full_time[4] = fullTime[6];

        if(buffer_index >= max_buffer_index) {
            buffer_index = 0;
            call HplAt45db.turnOn();
        }
        else{
            buffer_index += 1;
            call RxTimer.startOneShot(RX_DELAY);
            call TxTimer.startOneShot(rt);
        }
    }

    event void HplRV4162.setTimeDone(error_t err) {
        call HplAt45db.turnOn();
    }

    event void HplAt45db.turnedOn() {
        if(m_id_store.seed == 0) {
            call HplAt45db.read(0, &m_id_store, sizeof(id_store_t));
        }
        else {
            call HplAt45db.write_buffer_1(&buffer, writesize);
        }
    }

    event void HplAt45db.turnedOff() {
        if(page_count >= 4095) {
            call Leds.led0On();
        }
        else {
            setGuardTime();
            call RxTimer.startOneShot(RX_DELAY);
            call TxTimer.startOneShot(2000 + guard);
        }
    }

    event void HplAt45db.read_done(void *rxBuffer, uint16_t rx_len) {
        call RandomMt.seed(m_id_store.seed);
        opo_data->tx_id = m_id_store.id;
        call HplAt45db.turnOff();
    }

    event void HplAt45db.write_buffer_1_done() {
        call HplAt45db.flush_buffer_1(page_count);
    }
    event void HplAt45db.write_buffer_2_done() {}

    event void HplAt45db.flush_buffer_1_done() {
        page_count += 1;
        call HplAt45db.turnOff();
    }
    event void HplAt45db.flush_buffer_2_done() {}

    event void CC2420Config.syncDone(error_t error) {}

    inline void setGuardTime() {
        guard = call RandomMt.rand32();
        guard = guard % 2000;
    }

    event void LedTimer.fired() {
        call HplAt45db.turnOn();
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
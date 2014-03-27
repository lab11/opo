#include "Opo2FlashStore.h"

module Opo2FlashStoreP {
    uses {
        interface Boot;
        interface Leds;
        interface RandomMt;
        interface Packet;
        interface Opo;
        interface HplMsp430GeneralIO as I2CSwitch;
        interface Timer<TMilli> as TxTimer;
        interface Timer<TMilli> as RxTimer;
        interface Timer<TMilli> as LedTimer;
        interface PacketAcknowledgements as PacketAcks;
        interface HplSST25VF064 as FlashHpl;
        interface HplRV4162;
        interface CC2420Config;
        interface CC2420Packet;
        interface Acks;
        interface SplitControl as RadControl;
        interface AMSend as OpoBaseSend;
        interface AMPacket;
    }
}

implementation {
    #define BUFFER_SIZE 50

    uint32_t RX_DELAY = 70;
    uint8_t i = 0;

    //config setup
    oconfig_t m_config;

    // RF packet setup
    message_t packet;
    message_t base_packet;
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
    uint8_t initial_time[8] = {0};

    // Flash Storage Stuff
    uint8_t buffer_index = 0;
    uint8_t max_buffer_index = 15;
    uint32_t page_count = 0;
    oflash_base_msg_t buffer[16];

    uint32_t read_page_count = 0;
    uint8_t read_buffer_index = 0;
    bool read_new_page = FALSE;
    bool read_buffer_has_data = FALSE;
    oflash_base_msg_t read_buffer[16];

    uint32_t buffer_size = 256;

    // id and seed
    id_store_t m_id_store;

    enum {BASE_SEND, BASE_SEND_DONE, FLASH_STORE, FLASH_ERASE, BOOT, RANGING, WRITE_CONFIG, OFS_RECEIVE, OFS_TX, OFS_IDLE} m_state = BOOT;

    void setGuardTime();
    void getRemainingTimerTime();
    void restartOpo();

    event void Boot.booted() {
        m_id_store.seed = M_SEED;
        m_id_store.id = M_ID;

        m_config.valid = 0;
        m_config.reset_counter = 0;

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
        opo_data->tx_id = m_id_store.id;

        call AMPacket.setSource(&base_packet, m_id_store.id);
        call AMPacket.setSource(&packet, m_id_store.id);

        call I2CSwitch.makeOutput();
        call FlashHpl.turnOn();
    }

    event void RxTimer.fired() {
        call Opo.enable_receive();
    }

    event void TxTimer.fired() {
        bool should_tx = FALSE;
        opo_data->seq = seq;
        m_state = OFS_TX;
        atomic {
            if(call Opo.is_receiving()) {
                should_tx = FALSE;
                call Opo.disable_receive();
            }
            else {
                should_tx = TRUE;
                tx_fails += 1;
            }
        }
        if(should_tx == TRUE) {
            call I2CSwitch.set();
            call HplRV4162.readFullTime();
        }
        else {
            call TxTimer.startOneShot(guard + 75);
        }
    }

    event void Opo.transmit_done() {
        call RxTimer.stop();
        seq += 1;
        restartOpo();
    }

    event void Opo.transmit_failed() {
        tx_fails += 1;
        call TxTimer.startOneShot(guard + 75);
    }

    event void Opo.receive(uint16_t t_rf,
                           uint16_t t_ultrasonic,
                           message_t* msg) {

        call TxTimer.stop();
        tn = call TxTimer.getNow();
        t0 = call TxTimer.gett0();
        dt = call TxTimer.getdt();
        getRemainingTimerTime();

        opo_rx_data = call Packet.getPayload(msg, sizeof(oflash_msg_t));
        opo_data->last_tx_id = opo_rx_data->tx_id;
        opo_data->t_rf = t_rf;
        opo_data->t_ultrasonic = t_ultrasonic;

        if (opo_rx_data->tx_id == m_id_store.id) {
            m_state = BASE_SEND;
            for(i=0;i<8;i++) {
                initial_time[i] = opo_rx_data->m_full_time[i];
            }
            call I2CSwitch.set();
            call HplRV4162.setTime(initial_time);
        }
        else if (t_ultrasonic > t_rf) {
            buffer[buffer_index].ultrasonic_rf_dt = t_ultrasonic - t_rf;
            buffer[buffer_index].tx_seq = opo_rx_data->seq;
            buffer[buffer_index].tx_id = opo_rx_data->tx_id;
            buffer[buffer_index].rx_fails = rx_fails;
            buffer[buffer_index].tx_seq = opo_rx_data->seq;
            buffer[buffer_index].rssi = call CC2420Packet.getRssi(msg);
            buffer[buffer_index].reset_counter = m_reset_counter;
            for(i=0;i<8;++) {
                buffer[buffer_index].tx_full_time[i] = opo_rx_data->m_full_time[i];
            }
            call I2CSwitch.set();
            call HplRV4162.readFullTime();
        }
        else {
            call RxTimer.startOneShot(RX_DELAY);
            call TxTimer.startOneShot(rt);
        }
    }

    event void Opo.receive_failed(uint8_t rx_status) {
        rx_fails += 1;
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void Opo.enable_receive_failed() {
        enable_rx_fails += 1;
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void HplRV4162.readFullTimeDone(error_t err, uint8_t *fullTime) {
        // read time and store to buffer;
        call I2CSwitch.clr();
        if(m_state == OFS_RECEIVE) {
            for(i=0;i<8;i++) {
                buffer[buffer_index].full_time[i] = fullTime[i];
            }

            if(buffer_index >= max_buffer_index) {
                buffer_index = 0;
                call FlashHpl.turnOn();
            }
            else {
                buffer_index += 1;
                call RxTimer.startOneShot(RX_DELAY);
                call TxTimer.startOneShot(rt);
            }
        }
        else if (m_state == OFS_TX) {
            for(i=0;i<8;i++) {
                opo_data->m_full_time[i] = fullTime[i];
            }
            call Opo.transmit(&packet, sizeof(oflash_msg_t));
        }
    }

    event void HplRV4162.setTimeDone(error_t err) {
        if(m_state == ERASED || m_state == RESET) {
            call RandomMt.seed(m_id_store.seed);
            call I2CSwitch.clr();
            restartOpo();
        } else if (m_state == BASE_SEND) {
            call I2CSwitch.clr();
            call FlashHpl.turnOn();
        }
    }

    event void FlashHpl.turnedOn() {
        if(m_state == BOOT) {
            call FlashHpl.read(read_page_count, &read_buffer, sizeof(read_buffer));
        }
        else if(m_state == BASE_SEND) {
            call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));
        }
        else if(m_state == BASE_SEND_DONE) {
            call FlashHpl.chip_erase();
        }
        else if(m_state == OFS_RECEIVE) {
            call FlashHpl.page_program(page_count*256, &buffer, buffer_size);
        }
    }

    event void FlashHpl.chip_erase_done() {
        if(m_state == BASE_SEND_DONE) {
            call FlashHpl.turnOff();
        }
    }

    event void FlashHpl.read_done(void *rxBuffer, uint32_t rx_len) {
        if(m_state == BOOT) {
            if(read_buffer[0].tx_id < 255) {
                m_state = RESET;
                for(i=0;i<8;i++) {
                    initial_time[i]=0;
                }
                page_count += 1;
                read_page_count += 1;

                if(read_buffer[max_buffer_index].tx_id < 255) {
                    m_reset_counter = read_buffer[max_buffer_index].reset_counter + 1;
                    call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));
                }
                else if(read_buffer[0].tx_id == 0) {
                    m_reset_counter = read_buffer[0].reset_counter + 1;

                    buffer[0].tx_id = 0;
                    buffer[0].reset_counter = m_reset_counter;

                    call FlashHpl.page_program(page_count*256, &buffer, sizeof(buffer));
                }
            } else {
                m_state = ERASED;
                buffer[0].tx_id = 0;
                buffer[0].reset_counter = 0;
                call FlashHpl.write(0, &buffer, sizeof(buffer));
            }
        }
        else if (m_state == RESET) {
            if(read_buffer[0].tx_id < 255) {
                page_count += 1;
                read_page_count += 1;

                if(page_count > last_page) {
                    should_store = FALSE;
                    page_count = last_page;
                    read_page_count = 0;
                    call FlashHpl.turnOff();
                }
                else if(read_buffer[max_buffer_index].tx_id < 255) {
                    m_reset_counter = read_buffer[max_buffer_index].reset_counter + 1;
                    call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));
                }
                else if(read_buffer[0].tx_id == 0) {
                    m_reset_counter = read_buffer[0].reset_counter + 1;
                    buffer[0].tx_id = 0;
                    buffer[0].reset_counter = m_reset_counter;
                    call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));
                }
            } else {
                buffer[0].tx_id = 0;
                buffer[0].reset_counter = 0;
                call FlashHpl.write(256*page_count, &buffer, sizeof(buffer));
            }

        } else if (m_state == BASE_SEND) {
            // Reading data to send to base station
            read_page_count += 1;
            call FlashHpl.turnOff();
        }
    }

    event void FlashHpl.read_sid_done(void *rxBuffer, uint8_t rx_len) {}
    event void FlashHpl.program_sid_done(void *txBuffer, uint8_t tx_len) {}

    event void FlashHpl.page_program_done(void *txBuffer, uint32_t len) {
        if(m_state == ERASED || m_state == RESET) {
            page_count += 1;
            reading_page_count += 1;
            call FlashHpl.turnOff();
        }
        else if(m_state == OFS_RECEIVE) {
            page_count += 1;
            call FlashHpl.turnOff();
        }
    }

    event void FlashHpl.turnedOff() {
        if(m_state == ERASED || m_state == RESET) {
            call I2CSwitch.set();
            call HplRV4162.setTime(initial_time);
        }
        else if(m_state == BASE_SEND) {
            call RadControl.start();
        }
        else if(m_state == OFS_RECEIVE) {
            m_state = OFS_IDLE;
            restartOpo();
        }
    }

    event void RadControl.startDone() {
        /*
            Check to make sure that we want to send data to the base station, and not just
            conducting an opo transaction.

            Channel should already bet set to OPO_BASE before turning on the radio.
        */
        if(m_state == BASE_SEND) {
            base_data->tx_id = read_buffer[0].tx_id;
            base_data->ultrasonic_rf_dt = read_buffer[0].ultrasonic_rf_dt;
            base_data->rssi = read_buffer[0].rssi;
            base_data->tx_seq = read_buffer[0].tx_seq;
            base_data->rx_fails = read_buffer[0].rx_fails;
            base_data->reset_counter = read_buffer[0].reset_counter;
            for(i=0; i<8; i++) {
                base_data->full_time[i] = read_buffer[0].full_time[i];
                base_data->tx_full_time[i] = read_buffer[0].tx_full_time[i];
            }
            call OpoBaseSend.send(1, &packet, sizeof(opo_flash_store_base_rf_msg));
        }
    }
    event void RadControl.stopDone() {
        if(m_state == BASE_SEND) {
            if(read_page_count >= page_count) {
                m_state = BASE_SEND_DONE;
            }
            call FlashHpl.on();
        }
    }

    event void OpoBaseSend.sendDone(message_t *msg, error_t error) {
        if(call Acks.wasAcked(msg)) {
            read_buffer_index += 1;
            if(read_buffer_index >= buffer_size) {
                read_buffer_index = 0;
                call RadControl.stop();
            } else {
                base_data->tx_id = read_buffer[read_buffer_index].tx_id;
                base_data->ultrasonic_rf_dt = read_buffer[read_buffer_index].ultrasonic_rf_dt;
                base_data->rssi = read_buffer[read_buffer_index].rssi;
                base_data->tx_seq = read_buffer[read_buffer_index].tx_seq;
                base_data->rx_fails = read_buffer[read_buffer_index].rx_fails;
                base_data->reset_counter = read_buffer[0].reset_counter;
                for(i=0; i<8; i++) {
                    base_data->full_time[i] = read_buffer[read_buffer_index].full_time[i];
                    base_data->tx_full_time[i] = read_buffer[read_buffer_index].tx_full_time[i];
                }
            }
        }
        else {
            call OpoBaseSend.send(1, &basePacket, sizeof(oflash_base_rf_msg_t));
        }
    }

    event void CC2420Config.syncDone(error_t error) {}

    event void LedTimer.fired() {
        call FlashHpl.turnOn();
    }

    inline void setGuardTime() {
        guard = call RandomMt.rand32();
        guard = guard % 2000;
    }

    inline void restartOpo() {
        setGuardTime();
        call TxTimer.startOneShot(1000 + guard);
        call RxTimer.startOneShot(RX_DELAY);
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
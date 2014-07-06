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
        interface Timer<TMilli> as BaseTimer;
        interface Timer<TMilli> as BaseDelayTimer;
        interface PacketAcknowledgements as PacketAcks;
        interface HplSST25VF064 as FlashHpl;
        interface HplRV4162;
        interface CC2420Config;
        interface CC2420Packet;
        interface SplitControl as RadControl;
        interface AMSend as OpoBaseSend;
        interface AMSend as OpoBaseCheck;
        interface Receive as OpoBaseCheckReceive;
        interface AMPacket;
    }
}

implementation {
    #define BUFFER_SIZE 7

    uint32_t RX_DELAY = 70;
    uint16_t i = 0;

    //config setup
    oconfig_t m_config;

    // RF packet setup
    message_t packet;
    message_t base_packet;
    message_t base_check_packet;
    oflash_msg_t *opo_data;
    oflash_msg_t *opo_rx_data;
    oflash_base_rf_msg_t *base_data;
    oflash_base_check_msg_t *base_checker;

    // Base Send stuff
    uint16_t tx_count = 0;
    uint16_t no_ack_count = 0;
    uint8_t should_base_store = 0;
    oflash_base_check_msg_t *check_return = 0;

    // Timer delays
    uint32_t guard = 0;
    uint32_t dt = 0;
    uint32_t t0 = 0;
    uint32_t tn = 0;
    uint32_t rt = 0;

    // Opo Book keeping
    uint32_t rx_fails = 0;
    uint32_t enable_rx_fails = 0;
    uint32_t tx_fails = 0;
    uint32_t seq = 0;
    uint32_t m_reset_counter = 0;

    // Initial time for rtc
    uint8_t initial_time[8] = {0};

    // Flash Storage Stuff
    uint32_t max_pages = 32767;
    uint8_t buffer_index = 0;
    uint8_t max_buffer_index = 5;
    uint32_t page_count = 0; // current BLANK page to be written to
    oflash_base_msg_t buffer[6];

    uint32_t read_page_count = 0;
    int read_buffer_index = -1;
    bool read_new_page = FALSE;
    bool read_buffer_has_data = FALSE;
    bool should_store = TRUE;
    oflash_base_msg_t read_buffer[6];

    oflash_base_msg_t temp_buffer[6] = {0}; // used to write reset counter after a base send

    // id and seed
    id_store_t m_id_store;

    enum {BASE_CHECK,
          BASE_SEND_FAILED,
          BASE_SEND,
          BASE_SEND_DONE,
          FLASH_STORE,
          ERASED,
          BOOT,
          RANGING,
          RESET,
          OFS_RECEIVE, OFS_TX, OFS_IDLE} m_state = BOOT;

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

        base_data = (oflash_base_rf_msg_t*) call Packet.getPayload(&base_packet,
                                                                   sizeof(oflash_base_rf_msg_t));

        base_checker = (oflash_base_check_msg_t *) call Packet.getPayload(&base_check_packet,
                                                                          sizeof(base_check_packet));
        base_checker->m_id = M_ID;

        call PacketAcks.requestAck(&base_packet);
        call PacketAcks.requestAck(&base_check_packet);

        call I2CSwitch.makeOutput();
        call I2CSwitch.clr();
        call FlashHpl.turnOn();
    }

    event void RxTimer.fired() {
        call Opo.enable_receive();
    }

    event void TxTimer.fired() {
        bool should_tx = FALSE;
        opo_data->seq = seq;
        opo_data->reset_counter = m_reset_counter;
        m_state = OFS_TX;
        atomic {
            if(call Opo.is_receiving()) {
                should_tx = FALSE;
                tx_fails += 1;
            }
            else {
                call Opo.disable_receive();
                should_tx = TRUE;
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
        tx_count += 1;

        if(tx_count > 125) {
            if(read_page_count < page_count && page_count > 1) {
                call BaseDelayTimer.startOneShot(100);
            }
            else {
                tx_count = 0;
                restartOpo();
            }
        }
        else {
            restartOpo();
        }
    }

    event void Opo.transmit_failed() {
        tx_fails += 1;
        call TxTimer.startOneShot(guard + 75);
    }

    event void BaseDelayTimer.fired() {
        tx_count = 0;
        m_state = BASE_CHECK;
        call CC2420Config.setChannel(BASE_CHANNEL);
        call I2CSwitch.clr();
        call I2CSwitch.clr();
        call RadControl.start();
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

        if (t_ultrasonic > t_rf && call AMPacket.source(msg) != m_id_store.id) {
            opo_data->dt_ul_rf = t_ultrasonic - t_rf;
            opo_data->last_tx_id = call AMPacket.source(msg);
            buffer[buffer_index].ultrasonic_rf_dt = t_ultrasonic - t_rf;
            buffer[buffer_index].tx_seq = opo_rx_data->seq;
            buffer[buffer_index].tx_id = call AMPacket.source(msg);
            buffer[buffer_index].rx_fails = rx_fails;
            buffer[buffer_index].rssi = call CC2420Packet.getRssi(msg);
            buffer[buffer_index].m_reset_counter = m_reset_counter;
            buffer[buffer_index].tx_reset_counter = opo_rx_data->reset_counter;
            for(i=0;i<8;i++) {
                buffer[buffer_index].tx_full_time[i] = opo_rx_data->m_full_time[i];
            }
            m_state = OFS_RECEIVE;
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
                opo_data->last_full_time[i] = fullTime[i];
            }

            if(buffer_index >= max_buffer_index) {
                if(should_store == TRUE) {
                    buffer_index = 0;
                    call FlashHpl.turnOn();
                } else {
                    buffer_index = 0;
                    m_state = OFS_IDLE;
                    call RxTimer.startOneShot(RX_DELAY);
                    call TxTimer.startOneShot(rt);
                }
            }
            else {
                buffer_index += 1;
                m_state = OFS_IDLE;
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
            call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));
        }
        else if(m_state == BASE_SEND) {
            call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));
        }
        else if(m_state == BASE_SEND_DONE) {
            call FlashHpl.wrsr(0);
            call FlashHpl.chip_erase();
        }
        else if(m_state == OFS_RECEIVE) {
            call FlashHpl.wrsr(0);
            call FlashHpl.page_program(page_count*256, &buffer, sizeof(buffer));
        }
    }

    event void FlashHpl.chip_erase_done() {
        if(m_state == BASE_SEND_DONE) {
            for(i=0;i<6;i++) {
                temp_buffer[i].m_reset_counter = m_reset_counter;
            }
            read_page_count = 0;
            page_count = 0;
            call FlashHpl.page_program(page_count*256, &temp_buffer, sizeof(buffer));
        } else if(m_state == ERASED) {
            call FlashHpl.wrsr(0);
            call FlashHpl.page_program(0, &buffer, sizeof(buffer));
        }
    }

    event void FlashHpl.read_done(void *rxBuffer, uint32_t rx_len) {
        if(m_state == BOOT) {
            if(read_buffer[0].tx_id < 65535) {
                m_state = RESET;
                read_page_count += 1;
                page_count += 1;

                // Set new time to January 1st, 2000
                initial_time[0] = 0;
                initial_time[1] = 0;
                initial_time[2] = 0;
                initial_time[3] = 0;
                initial_time[4] = 7;
                initial_time[5] = 1;
                initial_time[6] = 1;
                initial_time[7] = 0;

                if(read_buffer[0].tx_id == 0) {
                    m_reset_counter = read_buffer[0].m_reset_counter + 1;
                }

                call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));

            } else {
                m_state = ERASED;
                buffer[0].tx_id = 0;
                buffer[0].m_reset_counter = 0;
                call FlashHpl.chip_erase();
            }
        }
        else if (m_state == RESET) {
            if(read_buffer[0].tx_id < 65535) {
                read_page_count += 1;
                page_count += 1;

                if(page_count > max_pages) {
                    should_store = FALSE;
                    page_count = max_pages;
                    read_page_count = 0;
                    call FlashHpl.turnOff();
                }

                if(read_buffer[0].tx_id == 0) {
                    m_reset_counter = read_buffer[0].m_reset_counter + 1;
                }

                call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));
            } else {
                buffer[0].tx_id = 0;
                buffer[0].m_reset_counter = m_reset_counter;
                call FlashHpl.wrsr(0);
                call FlashHpl.page_program(256*page_count, &buffer, sizeof(buffer));
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
        page_count += 1;
        if(m_state == ERASED || m_state == RESET) {
            read_page_count = 0;
            call FlashHpl.turnOff();
        }
        else if(m_state == OFS_RECEIVE) {
            call FlashHpl.turnOff();
        }
        else if(m_state == BASE_SEND_DONE) {
            call FlashHpl.turnOff();
        }
    }

    event void FlashHpl.turnedOff() {
        if(m_state == ERASED || m_state == RESET) {
            if(should_store == FALSE) {
                call Leds.led1On();
            }
            call I2CSwitch.set();
            call HplRV4162.setTime(initial_time);
        }
        else if(m_state == OFS_RECEIVE) {
            m_state = OFS_IDLE;
            restartOpo();
        }
        else if(m_state == BASE_SEND) {
            call I2CSwitch.clr();
            call I2CSwitch.clr();
            call RadControl.start();
        }
        else if(m_state == BASE_SEND_DONE) {
            call CC2420Config.setChannel(OPO_CHANNEL);
            m_state = OFS_IDLE;
            read_buffer_index = -1;
            should_base_store = 0;
            restartOpo();
        }
    }

    event void RadControl.startDone(error_t err) {
        /*
            Check to make sure that we want to send data to the base station, and not just
            conducting an opo transaction.

            Channel should already bet set to OPO_BASE before turning on the radio.
        */
        if(m_state == BASE_SEND) {
            if(read_buffer_index == -1) {
                read_buffer_index = 0;
            }
            base_data->tx_id = read_buffer[read_buffer_index].tx_id;
            base_data->ultrasonic_rf_dt = read_buffer[read_buffer_index].ultrasonic_rf_dt;
            base_data->rssi = read_buffer[read_buffer_index].rssi;
            base_data->tx_seq = read_buffer[read_buffer_index].tx_seq;
            base_data->rx_fails = read_buffer[read_buffer_index].rx_fails;
            base_data->m_reset_counter = read_buffer[read_buffer_index].m_reset_counter;
            base_data->tx_reset_counter = read_buffer[read_buffer_index].tx_reset_counter;
            for(i=0; i<8; i++) {
                base_data->full_time[i] = read_buffer[read_buffer_index].full_time[i];
                base_data->tx_full_time[i] = read_buffer[read_buffer_index].tx_full_time[i];
            }
            call OpoBaseSend.send(1, &base_packet, sizeof(oflash_base_rf_msg_t));
        }
        else if (m_state == BASE_CHECK) {
            call OpoBaseCheck.send(1, &base_check_packet, sizeof(oflash_base_check_msg_t));
        }
    }
    event void RadControl.stopDone(error_t err) {
        if(m_state == BASE_SEND) {
            if(read_page_count >= page_count) {
                call CC2420Config.setChannel(OPO_CHANNEL);
                m_state = BASE_SEND_DONE;
            }
            call FlashHpl.turnOn();
        }
        else if(m_state == BASE_SEND_FAILED) {
            call CC2420Config.setChannel(OPO_CHANNEL);
            no_ack_count = 0;
            should_base_store = 0;
            m_state = OFS_IDLE;
            restartOpo();
        }
    }

    event void OpoBaseCheck.sendDone(message_t *msg, error_t error) {
        call BaseTimer.startOneShot(150);
    }

    event message_t* OpoBaseCheckReceive.receive(message_t *msg,
                                                 void *payload,
                                                 uint8_t len) {
        atomic {
            if(should_base_store == 0) {
                should_base_store = 2;
            }
        }
        if (should_base_store == 2) {
            call BaseTimer.stop();
            check_return = (oflash_base_check_msg_t*) payload;
            if(check_return->m_id == M_ID) {
                call I2CSwitch.clr();
                call I2CSwitch.clr();
                m_state = BASE_SEND;
                if(read_buffer_index == -1) {
                    call RadControl.stop();
                }
                else {
                    base_data->tx_id = read_buffer[read_buffer_index].tx_id;
                    base_data->ultrasonic_rf_dt = read_buffer[read_buffer_index].ultrasonic_rf_dt;
                    base_data->rssi = read_buffer[read_buffer_index].rssi;
                    base_data->tx_seq = read_buffer[read_buffer_index].tx_seq;
                    base_data->rx_fails = read_buffer[read_buffer_index].rx_fails;
                    base_data->m_reset_counter = read_buffer[read_buffer_index].m_reset_counter;
                    base_data->tx_reset_counter = read_buffer[read_buffer_index].tx_reset_counter;
                    for(i=0; i<8; i++) {
                        base_data->full_time[i] = read_buffer[read_buffer_index].full_time[i];
                        base_data->tx_full_time[i] = read_buffer[read_buffer_index].tx_full_time[i];
                    }
                    call OpoBaseSend.send(1, &base_packet, sizeof(oflash_base_rf_msg_t));
                }
            }
            else {
                m_state = BASE_SEND_FAILED;
                call RadControl.stop();
            }
        }

        return msg;
    }

    event void OpoBaseSend.sendDone(message_t *msg, error_t error) {
        if(call PacketAcks.wasAcked(msg)) {
            no_ack_count = 0;
            read_buffer_index += 1;
            if(read_buffer_index > max_buffer_index) {
                read_buffer_index = -1;
                call RadControl.stop();
            } else {
                base_data->tx_id = read_buffer[read_buffer_index].tx_id;
                base_data->ultrasonic_rf_dt = read_buffer[read_buffer_index].ultrasonic_rf_dt;
                base_data->rssi = read_buffer[read_buffer_index].rssi;
                base_data->tx_seq = read_buffer[read_buffer_index].tx_seq;
                base_data->rx_fails = read_buffer[read_buffer_index].rx_fails;
                base_data->m_reset_counter = read_buffer[read_buffer_index].m_reset_counter;
                base_data->tx_reset_counter = read_buffer[read_buffer_index].tx_reset_counter;
                for(i=0; i<8; i++) {
                    base_data->full_time[i] = read_buffer[read_buffer_index].full_time[i];
                    base_data->tx_full_time[i] = read_buffer[read_buffer_index].tx_full_time[i];
                }
                call OpoBaseSend.send(1, &base_packet, sizeof(oflash_base_rf_msg_t));
            }
        }
        else {
            no_ack_count += 1;
            if (no_ack_count > 10) {
                m_state = BASE_SEND_FAILED;
                no_ack_count = 0;
                call RadControl.stop();
            } else {
                call OpoBaseSend.send(1, &base_packet, sizeof(oflash_base_rf_msg_t));
            }
        }
    }

    event void CC2420Config.syncDone(error_t error) {}

    event void BaseTimer.fired() {
        atomic {
            if(should_base_store == 0) {
                should_base_store = 1;
            }
        }
        if(should_base_store == 1) {
            m_state = BASE_SEND_FAILED;
            call RadControl.stop();
        }
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
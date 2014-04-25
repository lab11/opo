#include "Opo2FlashStore.h"

module Opo2FlashStoreP {
    uses {
        interface Boot;
        interface Leds;
        interface RandomMt;
        interface HplMsp430GeneralIO as I2CSwitch;
        interface Timer<TMilli> as LedTimer;
        interface HplSST25VF064 as FlashHpl;
        interface SplitControl as RadControl;
        interface AMSend as OpoBaseSend;
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
    oflash_msg_t *opo_data;
    oflash_msg_t *opo_rx_data;
    oflash_base_rf_msg_t *base_data;

    // Timer delays
    uint32_t guard = 0;
    uint32_t dt = 0;
    uint32_t t0 = 0;
    uint32_t tn = 0;
    uint32_t rt = 0;

    // Opo Bookkeeping
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
    uint8_t max_buffer_index = 6;
    uint32_t page_count = 0;
    oflash_base_msg_t buffer[7];

    uint32_t read_page_count = 0;
    uint8_t read_buffer_index = 0;
    bool read_new_page = FALSE;
    bool read_buffer_has_data = FALSE;
    bool should_store = TRUE;
    bool firstRead = TRUE;
    bool firstWrite = TRUE;
    oflash_base_msg_t read_buffer[7];

    // id and seed
    id_store_t m_id_store;

    enum {BASE_SEND, BASE_SEND_DONE, FLASH_STORE, ERASED, BOOT, RANGING, RESET, OFS_RECEIVE, OFS_TX, OFS_IDLE} m_state = BOOT;

    void setGuardTime();
    void getRemainingTimerTime();

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

        for(i=0;i<7;i++) {
            buffer[i].tx_seq = 2;
            buffer[i].rx_fails = 10;
            buffer[i].tx_id = 12;
            buffer[i].rssi = page_count;
            buffer[i].reset_counter = 16;
        }

        base_data = (oflash_base_rf_msg_t*) call OpoBaseSend.getPayload(&base_packet,
                                                                   sizeof(oflash_base_rf_msg_t));

        call FlashHpl.turnOn();
    }

    event void FlashHpl.turnedOn() {
        if(m_state == BOOT) {
            call FlashHpl.wrsr(0);
            call FlashHpl.chip_erase();
        }
        else if(m_state == OFS_TX) {
            call FlashHpl.wrsr(0);
            for(i=0;i<7;i++) {
                buffer[i].tx_seq = 1;
                buffer[i].rx_fails = 11;
                buffer[i].tx_id = 13;
                buffer[i].rssi = page_count;
                buffer[i].reset_counter = 17;
            }
            call FlashHpl.page_program(page_count*256, &buffer, sizeof(buffer));
        }
        else if(m_state == BASE_SEND) {
            call FlashHpl.read(read_page_count*256, &read_buffer, sizeof(read_buffer));
        }
    }

    event void FlashHpl.chip_erase_done() {
        if(m_state == BOOT || m_state == OFS_TX) {
            call FlashHpl.wrsr(0);
            call FlashHpl.page_program(page_count*256, &buffer, sizeof(buffer));
        }
    }

    event void FlashHpl.read_done(void *rxBuffer, uint32_t rx_len) {
        if (m_state == BASE_SEND) {
            read_page_count += 1;
            call FlashHpl.turnOff();
        }
    }

    event void FlashHpl.read_sid_done(void *rxBuffer, uint8_t rx_len) {}
    event void FlashHpl.program_sid_done(void *txBuffer, uint8_t tx_len) {}

    event void FlashHpl.page_program_done(void *txBuffer, uint32_t len) {
        if(m_state == BOOT) {
            page_count++;
            call FlashHpl.turnOff();
        }
        else if(m_state == OFS_TX) {
            call FlashHpl.turnOff();
        }
    }

    event void FlashHpl.turnedOff() {
        if(m_state == BOOT) {
            //call I2CSwitch.set();
            m_state = OFS_TX;
            call LedTimer.startOneShot(1000);
        }
        else if(m_state == OFS_TX) {
            m_state = BASE_SEND;
            call LedTimer.startOneShot(1000);
        }
        else if(m_state == BASE_SEND) {
            firstRead = TRUE;
            call I2CSwitch.clr();
            call RadControl.start();
        }
    }

    event void RadControl.startDone(error_t err) {
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
            call OpoBaseSend.send(1, &base_packet, sizeof(oflash_base_rf_msg_t));
        }
    }
    event void RadControl.stopDone(error_t err) {
        if(m_state == BASE_SEND) {
            if(read_page_count <= page_count) {
                call FlashHpl.turnOn();
            }
        }
    }

    event void OpoBaseSend.sendDone(message_t *msg, error_t error) {
            read_buffer_index += 1;
            if(read_buffer_index > 6) {
                read_buffer_index = 0;
                call RadControl.stop();
            } else {
                call Leds.led0Toggle();
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
                call OpoBaseSend.send(1, &base_packet, sizeof(oflash_base_rf_msg_t));
            }

    }


    event void LedTimer.fired() {
        call FlashHpl.turnOn();
    }

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

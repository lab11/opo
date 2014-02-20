#include "OpoFlashStoreReader.h"


module OpoFlashStoreReaderP {
    uses {
        interface Boot;
        interface Leds;
        interface AMSend;
        interface SplitControl as RfControl;
        interface Timer<TMilli> as ReadTimer;
        interface HplAt45db;
    }
}

implementation {

    // RF packet setup
    message_t packet;

    // id and seed
    id_store_t id_store;

    // Flash addr
    uint16_t page_count = 0;

    // Message Pointer
    oflash_base_rf_msg_t *data;

    // Read Buffer
    oflash_base_msg_t buffer[50];
    uint8_t buffer_index = 0;
    uint8_t buffer_size = 33;
    bool first_read = TRUE;

    uint8_t base_time[5] = {0};
    uint8_t current_time[5] = {0};
    bool base_read = TRUE;
    int i = 0;
    uint16_t count = 0;

    bool compare_times();

    event void Boot.booted() {
        id_store.seed = 0;
        id_store.id = 0;
        data = (oflash_base_rf_msg_t *) call AMSend.getPayload(&packet, sizeof(oflash_base_rf_msg_t));
        call HplAt45db.turnOn();
    }

    event void RfControl.startDone(error_t err) {
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
    }
    event void RfControl.stopDone(error_t err) {
        call HplAt45db.turnOn();
    }

    event void AMSend.sendDone(message_t *msg, error_t error) {
        //call Leds.led1Toggle();

        buffer_index += 1;
        if(buffer_index >= buffer_size) {
            buffer_index = 0;
            call RfControl.stop();
        }
        else {
            data->tx_id = buffer[buffer_index].tx_id;
            data->ultrasonic_rf_dt = buffer[buffer_index].ultrasonic_rf_dt;
            data->rssi = buffer[buffer_index].rssi;
            data->tx_seq = buffer[buffer_index].tx_seq;
            data->rx_fails = buffer[buffer_index].rx_fails;
            data->m_seq = buffer[buffer_index].m_seq;
            for(i=0;i<5;i++) {
                data->full_time[i] = buffer[buffer_index].full_time[i];
                current_time[i] = buffer[buffer_index].full_time[i];
            }

            if(compare_times()) {
                call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
            }
        }

    }

    event void HplAt45db.turnedOn() {
        if(first_read == TRUE) {
            call HplAt45db.read(0, &id_store, sizeof(id_store_t));
        }
        else {
            call HplAt45db.read(page_count, &buffer, sizeof(oflash_base_msg_t) * buffer_size);
        }
    }
    event void HplAt45db.turnedOff() {
        if(base_read == TRUE) {
                base_read = FALSE;
                call RfControl.start();
        }
        else if(compare_times()) {
            call RfControl.start();
        }
    }

    event void HplAt45db.read_done(void *rxBuffer, uint16_t rx_len) {
        uint8_t *m_addr = call HplAt45db.get_addr();
        page_count += 1;

        if(first_read == TRUE) {
            call Leds.led0On();
            first_read = FALSE;
            data->rx_id = id_store.id;
            call HplAt45db.read(page_count, &buffer, sizeof(oflash_base_msg_t) * buffer_size);
        }
        else {
            if(base_read == TRUE) {
                for(i=0;i<5;i++)
                    base_time[i] = buffer[0].full_time[i];
            }

            data->tx_id = buffer[0].tx_id;
            data->ultrasonic_rf_dt = buffer[0].ultrasonic_rf_dt;
            data->rssi = buffer[0].rssi;
            data->tx_seq = buffer[0].tx_seq;
            data->rx_fails = buffer[0].rx_fails;
            data->m_seq = buffer[0].m_seq;
            for(i=0;i<5;i++) {
                data->full_time[i] = buffer[0].full_time[i];
                current_time[i] = buffer[0].full_time[i];
            }
            for(i=0;i<3;i++) {
                data->addr[i] = m_addr[i];
            }
            data->page_addr = page_count - 1;

            call HplAt45db.turnOff();
        }
    }

    event void HplAt45db.write_buffer_1_done() {}
    event void HplAt45db.write_buffer_2_done() {}
    event void HplAt45db.flush_buffer_1_done() {}
    event void HplAt45db.flush_buffer_2_done() {}

    event void ReadTimer.fired() {
        call HplAt45db.turnOn();
    }

    bool compare_times() {
        for(i=4;i>=0;i--) {
            if(current_time[i] > base_time[i]) {
                return TRUE;
            }
            if(current_time[i] < base_time[i]) {
                return FALSE;
            }
        }
        return FALSE;
    }

}
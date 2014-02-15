#include "OpoFlashStoreReader.h"


module OpoFlashStoreReaderP {
    uses {
        interface Boot;
        interface Leds;
        interface AMSend;
        interface SplitControl as FlashPower;
        interface SplitControl as RfControl;
        interface Timer<TMilli> as ReadTimer;
        interface BlockRead;
    }
}

implementation {

    // RF packet setup
    message_t packet;

    // id and seed
    id_store_t id_store;

    // Flash addr
    storage_addr_t flash_addr = 0;

    // Message Pointer
    oflash_base_rf_msg_t *data;

    // Read Buffer
    oflash_base_msg_t buffer[35];
    uint8_t buffer_index = 0;
    uint8_t buffer_size = 20;
    uint32_t read_counter = 0;

    uint8_t base_time[8] = {0};
    uint8_t current_time[8] = {0};
    bool base_read = FALSE;
    int i = 0;
    uint16_t count = 0;

    bool compare_times();

    event void Boot.booted() {
        id_store.seed = 0;
        id_store.id = 0;
        data = (oflash_base_rf_msg_t *) call AMSend.getPayload(&packet, sizeof(oflash_base_rf_msg_t));
        call RfControl.start();
    }

    event void RfControl.startDone(error_t err) {
        call FlashPower.start();
    }
    event void RfControl.stopDone(error_t err) {}

    event void AMSend.sendDone(message_t *msg, error_t error) {
        call Leds.led1Toggle();
        buffer_index += 1;
        if(buffer_index == buffer_size) {
            call ReadTimer.startOneShot(2000);
        }
        else {
            data->tx_id = buffer[buffer_index].tx_id;
            data->ultrasonic_rf_dt = buffer[buffer_index].ultrasonic_rf_dt;
            data->rssi = buffer[buffer_index].rssi;
            data->seq = buffer[buffer_index].seq;
            data->rx_fails = buffer[buffer_index].rx_fails;
            data->enable_rx_fails = buffer[buffer_index].enable_rx_fails;
            data->tx_fails = buffer[buffer_index].tx_fails;
            for(i=0;i<8;i++) {
                data->full_time[i] = buffer[buffer_index].full_time[i];
                current_time[i] = buffer[buffer_index].full_time[i];
            }

            if(compare_times()) {
                call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
            }
            else if(count < 90) {
                count += 1;
                call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
                call Leds.led0On();
            }
            else {
                call Leds.led0On();
            }
        }

    }

    event void FlashPower.startDone(error_t err) {
        call BlockRead.read(flash_addr, &id_store, sizeof(id_store_t));
    }


    event void BlockRead.readDone(storage_addr_t addr,
                                  void *buf,
                                  storage_len_t len,
                                  error_t error) {
        if(read_counter == 0) {
            flash_addr += sizeof(id_store_t);
            read_counter += 1;
            data->rx_id = id_store.id;
            call BlockRead.read(flash_addr, &buffer, sizeof(oflash_base_msg_t) * buffer_size);
        }
        else{
            flash_addr = read_counter * 528;
            read_counter += 1;

            if(base_read == FALSE) {
                for(i=0;i++;i<8) {
                    base_time[i] = buffer[0].full_time[i];
                }
            }

            data->tx_id = buffer[0].tx_id;
            data->ultrasonic_rf_dt = buffer[0].ultrasonic_rf_dt;
            data->rssi = buffer[0].rssi;
            data->seq = buffer[0].seq;
            data->rx_fails = buffer[0].rx_fails;
            data->enable_rx_fails = buffer[0].enable_rx_fails;
            data->tx_fails = buffer[0].tx_fails;
            for(i=0;i<8;i++) {
                data->full_time[i] = buffer[0].full_time[i];
                current_time[i] = buffer[0].full_time[i];
            }

            if(base_read == FALSE) {
                base_read = TRUE;
                call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
            }
            else if(compare_times()) {
                call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
            }
            else if(count < 90) {
                count++;
                call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
            }
            else {
                call Leds.led1On();
            }
        }
    }

    event void ReadTimer.fired() {
        call BlockRead.read(flash_addr, &buffer, sizeof(oflash_base_msg_t) * buffer_size);
    }

    bool compare_times() {
        for(i=7;i>=0;i--) {
            if(current_time[i] > base_time[i]) {
                return TRUE;
            }
            if(current_time[i] < base_time[i]) {
                return FALSE;
            }
        }
        return FALSE;
    }

    event void FlashPower.stopDone(error_t err) {}
    event void BlockRead.computeCrcDone(storage_addr_t addr,
                                        storage_len_t len,
                                        uint16_t crc,
                                        error_t error) {}

}
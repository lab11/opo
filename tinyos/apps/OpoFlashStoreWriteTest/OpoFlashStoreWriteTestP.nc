#include "OpoFlashStoreWriteTest.h"


module OpoFlashStoreWriteTestP {
    uses {
        interface Boot;
        interface Leds;
        interface HplAt45db;
        interface Timer<TMilli> as WaitTimer;
    }
}

implementation {

    // id and seed
    id_store_t id_store;

    // Flash addr
    uint16_t page_count = 0;

    // Message Pointer
    oflash_base_rf_msg_t *data;

    // Read Buffer
    oflash_base_msg_t buffer[60];
    uint8_t buffer_index = 0;
    uint8_t buffer_size = 33;
    bool first_read = TRUE;

    int i = 0;
    uint16_t count = 0;

    event void Boot.booted() {
        id_store.seed = 0;
        id_store.id = 0;

        for(i=0;i<buffer_size;i++) {
            buffer[i].tx_seq = 5;
            buffer[i].m_seq = 2;
            buffer[i].tx_id = 27;
            buffer[i].ultrasonic_rf_dt = 22;
            buffer[i].rssi = 42;
            buffer[i].rx_fails = 30;
            buffer[i].full_time[0] = 0;
            buffer[i].full_time[1] = 1;
            buffer[i].full_time[2] = 2;
            buffer[i].full_time[3] = 3;
            buffer[i].full_time[4] = 4;
        }
        call Leds.led0On();
        call HplAt45db.turnOn();
    }

    event void HplAt45db.turnedOn() {
        call HplAt45db.read(page_count, &id_store, sizeof(id_store_t));
    }
    event void HplAt45db.turnedOff() {
        call Leds.led0On();
        call Leds.led1On();
    }

    event void HplAt45db.read_done(void *rxBuffer, uint16_t rx_len) {
        page_count += 1;
        call HplAt45db.write_buffer_1(&buffer, sizeof(oflash_base_msg_t) * buffer_size);
    }

    event void HplAt45db.write_buffer_1_done() {
        //call Leds.led1On();
        call HplAt45db.flush_buffer_1(page_count);
    }
    event void HplAt45db.write_buffer_2_done() {}
    event void HplAt45db.flush_buffer_1_done() {
        call Leds.led1Toggle();
        page_count += 1;
        if(page_count < 500)
            call HplAt45db.write_buffer_1(&buffer, sizeof(oflash_base_msg_t) * buffer_size);
        else
            call HplAt45db.turnOff();
    }
    event void HplAt45db.flush_buffer_2_done() {}

    event void WaitTimer.fired() {
        call HplAt45db.turnOn();
    }

}
#include "OpoFlashStoreIDWriter.h"

module OpoFlashStoreIDWriterP {
    uses {
        interface Boot;
        interface Leds;
        interface AMSend;
        interface HplAt45db;
        interface SplitControl as RfControl;
        interface Timer<TMilli> as SendTimer;
    }
}

implementation {

    // RF packet setup
    message_t packet;
    id_store_t *data;

    // id and seed
    id_store_t m_id_store;


    id_store_t read_id;


    void setGuardTime();
    void getRemainingTimerTime();

    event void Boot.booted() {
        m_id_store.seed = M_SEED;
        m_id_store.id = M_ID;
        read_id.seed = 0;
        read_id.id = 0;
        data = (id_store_t *) call AMSend.getPayload(&packet, sizeof(id_store_t));
        call HplAt45db.turnOn();
    }

    event void RfControl.startDone(error_t err) {
        data->id = read_id.id;
        data->seed = read_id.seed;
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(id_store_t));
    }
    event void RfControl.stopDone(error_t err) {

    }

    event void AMSend.sendDone(message_t *msg, error_t error) {
        call SendTimer.startOneShot(1000);
    }

    event void SendTimer.fired() {
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(id_store_t));
    }

    event void HplAt45db.turnedOn() {
        call HplAt45db.write_buffer_1(&m_id_store, sizeof(m_id_store));

    }
    event void HplAt45db.turnedOff() {
        call Leds.led0On();
        call RfControl.start();
    }

    event void HplAt45db.read_done(void *rxBuffer, uint16_t rx_len) {
        call Leds.led1On();
        call HplAt45db.turnOff();
    }

    event void HplAt45db.write_buffer_1_done() {
        uint16_t page_addr = 0;
        call HplAt45db.flush_buffer_1(page_addr);

    }
    event void HplAt45db.write_buffer_2_done() {

    }

    event void HplAt45db.flush_buffer_1_done() {
        uint16_t page_addr = 0;
        call HplAt45db.read(page_addr, &read_id, sizeof(m_id_store));

    }
    event void HplAt45db.flush_buffer_2_done() {

    }

}
#include "Opo2FlashStoreIDWriter.h"

module Opo2FlashStoreIDWriterP {
    uses {
        interface Boot;
        interface Leds;
        interface AMSend;
        interface HplSST25VF064 as FlashHpl;
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
    id_store_t read_id_store;
    uint16_t m_id = 0;
    uint16_t read_id = 0;
    uint32_t seed = 0;

    uint8_t sidBuffer[35] = {0};

    void setGuardTime();
    void getRemainingTimerTime();

    event void Boot.booted() {
        uint8_t i = 0;
        for(i=0;i<35;i++)
            sidBuffer[i] = 0;

        m_id_store.id = M_ID;
        m_id_store.seed = M_SEED;
        read_id_store.id = 0;
        read_id_store.seed = 0;

        data = (id_store_t *) call AMSend.getPayload(&packet, sizeof(id_store_t));
        call FlashHpl.turnOn();
    }

    event void RfControl.startDone(error_t err) {
        data->id = read_id_store.id;
        data->seed = read_id_store.seed;
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(id_store_t));
    }
    event void RfControl.stopDone(error_t err) {}

    event void AMSend.sendDone(message_t *msg, error_t error) {
        call SendTimer.startOneShot(1000);
    }

    event void SendTimer.fired() {
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(id_store_t));
    }

    event void FlashHpl.turnedOn() {
        uint8_t test_id[24] = {0};
        uint8_t i = 0;
        call FlashHpl.wrsr(0);
        call FlashHpl.chip_erase();
        //call FlashHpl.read_sid(0, &sidBuffer, 8);
    }
    event void FlashHpl.turnedOff() {
        call RfControl.start();
    }

    event void FlashHpl.program_sid_done(void *txBuffer, uint8_t len) {
        call FlashHpl.read_sid(0, &sidBuffer, 8);
    }

    event void FlashHpl.read_sid_done(void *rxBuffer, uint8_t rx_len) {
        uint8_t *converter = &sidBuffer[0];
        uint8_t i = 0;
        uint8_t status = call FlashHpl.read_status_register();
        if(status == 0) {
            call Leds.led0On();
        }

        for(i=4; i < 8; i++) {
            seed <<= 8;
            seed += converter[i];
        }
        call FlashHpl.chip_erase();
    }

    event void FlashHpl.read_done(void *rxBuffer, uint32_t rx_len) {
        call FlashHpl.turnOff();
    }

    event void FlashHpl.page_program_done(void *txBuffer, uint32_t len) {
        uint32_t p = 0;
        call FlashHpl.read(p, &read_id_store, sizeof(read_id_store));
    }

    event void FlashHpl.chip_erase_done() {
        uint32_t test_page = 0;
        call FlashHpl.page_program(test_page, &m_id_store, sizeof(m_id_store));
    }

}
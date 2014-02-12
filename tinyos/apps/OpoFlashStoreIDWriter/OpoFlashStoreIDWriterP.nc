#include "OpoFlashStoreIDWriter.h"

module OpoFlashStoreIDWriterP {
    uses {
        interface Boot;
        interface Leds;
        interface AMSend;
        interface SplitControl as FlashPower;
        interface SplitControl as RfControl;
        interface Timer<TMilli> as SendTimer;
        interface BlockRead;
        interface BlockWrite;
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
        call FlashPower.start();
    }

    event void RfControl.startDone(error_t err) {
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(id_store_t));
    }
    event void RfControl.stopDone(error_t err) {

    }

    event void AMSend.sendDone(message_t *msg, error_t error) {
        call SendTimer.startOneShot(1000);
    }

    event void SendTimer.fired() {
        call Leds.led1Toggle();
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(id_store_t));
    }

    event void FlashPower.startDone(error_t err) {
        call BlockWrite.erase();
    }

    event void BlockWrite.eraseDone(error_t err) {
        call BlockWrite.write(0, &m_id_store, sizeof(id_store_t));
    }

    event void BlockWrite.writeDone(storage_addr_t addr,
                                                      void *buf,
                                                      storage_len_t len,
                                                      error_t error) {
        call BlockWrite.sync();
    }

    event void BlockWrite.syncDone(error_t err) {
        call BlockRead.read(0, &read_id, sizeof(id_store_t));
    }


    event void BlockRead.readDone(storage_addr_t addr,
                                                    void *buf,
                                                    storage_len_t len,
                                                    error_t error) {
        data->id = read_id.id;
        data->seed = read_id.seed;
        call RfControl.start();
    }



    event void FlashPower.stopDone(error_t err) {}

    event void BlockRead.computeCrcDone(storage_addr_t addr,
                                                                storage_len_t len,
                                                                uint16_t crc,
                                                                error_t error) {}

}
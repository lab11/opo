#include "OpoFlashStoreEraser.h"

module OpoFlashStoreEraserP {
    uses {
        interface Boot;
        interface Leds;
        interface SplitControl as FlashPower;
        interface BlockWrite;
    }
}

implementation {
    uint8_t buffer[528] = {0};
    //storage_addr_t flash_addr = sizeof(id_store_t);
    storage_addr_t flash_addr = sizeof(id_store_t);
    uint32_t page_count = 0;

    event void Boot.booted() {
        call FlashPower.start();
    }

    event void FlashPower.startDone(error_t err) {
        call BlockWrite.erase();
    }

    event void BlockWrite.eraseDone(error_t err) {
        call BlockWrite.write(flash_addr, &buffer, 512 - sizeof(id_store_t));
    }

    event void BlockWrite.writeDone(storage_addr_t addr,
                                    void *buf,
                                    storage_len_t len,
                                    error_t error) {
        call BlockWrite.sync();
    }

    event void BlockWrite.syncDone(error_t err) {
        page_count += 1;
        flash_addr = page_count * 512;
        if(page_count < 4096) {
            call Leds.led1Toggle();
            call BlockWrite.write(flash_addr, &buffer, 512);
        } else {
            call Leds.led0On();
            call Leds.led1On();
        }
    }

    event void FlashPower.stopDone(error_t err) {}

}
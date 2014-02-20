#include "OpoFlashStoreEraser.h"

module OpoFlashStoreEraserP {
    uses {
        interface Boot;
        interface Leds;
        interface HplAt45db;
    }
}

implementation {
    uint8_t buffer[528] = {0};
    uint32_t page_count = 1;

    event void Boot.booted() {
        call HplAt45db.turnOn();
    }
    event void HplAt45db.turnedOn() {
        call HplAt45db.write_buffer_1(&buffer, 528);
    }
    event void HplAt45db.turnedOff() {}

    event void HplAt45db.read_done(void *rxBuffer, uint16_t rx_len) {}

    event void HplAt45db.write_buffer_1_done() {
        call HplAt45db.flush_buffer_1(page_count);
    }
    event void HplAt45db.write_buffer_2_done() {}

    event void HplAt45db.flush_buffer_1_done() {
        page_count += 1;
        if(page_count < 4096) {
            call Leds.led1Toggle();
            call HplAt45db.write_buffer_1(&buffer, 528);
        }
        else {
            call Leds.led0On();
            call Leds.led1On();
        }
    }
    event void HplAt45db.flush_buffer_2_done() {}

}
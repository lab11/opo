#include "Opo2FlashTest.h"
module Opo2FlashTestP {
	uses {
		interface Boot;
		interface Leds;
		interface AMSend;
		interface SplitControl;
		interface HplSST25VF064 as FlashHpl;
        interface Timer<TMilli> as FlashTimer;
	}
}
implementation {
    int i = 0;
    message_t packet;
    opo_flash_t *p;
    uint8_t txBuffer[25] = {0};
    uint8_t sidBuffer[25] = {0};
    uint8_t readBuffer[25] = {0};

    event void Boot.booted() {
        for(i=0; i < 25; i++) {
            sidBuffer[i] = 0;
            txBuffer[i] = 0;
        }
        p = (opo_flash_t*) call AMSend.getPayload(&packet, sizeof(opo_flash_t));
        call FlashHpl.turnOn();
    }

    event void FlashHpl.turnedOn() {
    	uint16_t len = 8;
    	uint8_t addr[3] = {0,0,0};
        uint8_t truth_counter = 0;

    	call FlashHpl.read_sid(addr, &sidBuffer[0], len);

    	for(i=0; i < len; i++) {
    		p->sid[i] = sidBuffer[i];
            txBuffer[i] = sidBuffer[i];

            if(sidBuffer[i] != 0) {
                call Leds.led0On();
            }
    	}

        call FlashHpl.write_enable();

        call FlashHpl.chip_erase();
        call FlashHpl.page_program(addr, &txBuffer[0], len);
        call FlashHpl.read(addr, &readBuffer[0], len);

        for(i=0; i < len; i++) {
            if(sidBuffer[i] == readBuffer[i]) {
                truth_counter += 1;
            }
        }

        if(truth_counter >= 3) {
            call Leds.led1On();
        }

    	call SplitControl.start();
    }

    event void FlashHpl.turnedOff() {

    }


    event void AMSend.sendDone(message_t *msg, error_t err) {
        call FlashTimer.startOneShot(1500);
    }

    event void SplitControl.startDone(error_t err) {
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_flash_t));
    }

    event void FlashTimer.fired() {
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_flash_t));
    }

    event void SplitControl.stopDone(error_t err) {}
}
#include "Opo2FlashTest.h"
module Opo2FlashTestP {
	uses {
		interface Boot;
		interface Leds;
		//interface AMSend;
		//interface SplitControl;
		interface HplSST25VF064 as FlashHpl;
	}
}
implementation {
    int i = 0;
    //message_t packet;
    //opo_flash_t *p;
    uint8_t txBuffer[64] = {0};
    uint8_t rxBuffer[64] = {0};

    event void Boot.booted() {
        //p = (opo_flash_t*) call AMSend.getPayload(&packet, sizeof(opo_flash_t));
        call FlashHpl.turnOn();
    }

    event void FlashHpl.turnedOn() {
    	uint16_t len = 8;
    	uint8_t addr[3];
    	addr[0] = 0;
    	addr[1] = 0;
    	addr[2] = 0;

    	call FlashHpl.read_sid(addr, &txBuffer[0], &rxBuffer[0], len);
    	//for(i=0; i < len; i++) {
    	//	p->sid[i] = rxBuffer[i];
    	//}
    	//call SplitControl.start();
    }

    event void FlashHpl.turnedOff() {

    }

/*
    event void AMSend.sendDone(message_t *msg, error_t err) {

    }

    event void SplitControl.startDone(error_t err) {

        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_flash_t));
    }

    event void SplitControl.stopDone(error_t err) {} */
}
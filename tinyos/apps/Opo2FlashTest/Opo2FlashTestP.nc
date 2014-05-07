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
    uint8_t sidBuffer[25]  = {0};
    uint8_t readBuffer[25] = {0};
    uint8_t writeSidBuffer[8] = {1,2,3,22,5,6,7,8};
    uint8_t addr[3] = {0,0,0};
    uint8_t status_register = 0;
    uint16_t len = 8;
    bool w = FALSE;

    event void Boot.booted() {
        for(i=0; i < 25; i++) {
            sidBuffer[i]  = 0;
            readBuffer[i] = 0;
        }
        p = (opo_flash_t*) call AMSend.getPayload(&packet, sizeof(opo_flash_t));
        call FlashHpl.turnOn();
    }

    event void FlashHpl.turnedOn() {
        call FlashHpl.wrsr(0);
        call FlashHpl.chip_erase();
    }

    event void FlashHpl.turnedOff() {
        call Leds.led1On();
    }

    event void FlashHpl.program_sid_done(void *data, uint8_t tx_len) {
        call FlashHpl.read_sid(0, &sidBuffer, 16);
    }
    event void FlashHpl.read_sid_done(void *rxBuffer, uint8_t rx_len) {
        for(i=0; i < 16; i++) {
            p->sid[i] = sidBuffer[i];
        }
        call FlashHpl.wrsr(0);
        call FlashHpl.chip_erase();
    }

    event void FlashHpl.chip_erase_done() {
        call Leds.led0On();
        call FlashHpl.turnOff();
    }

    event void FlashHpl.page_program_done(void *txBuffer, uint32_t tx_len) {
        w = TRUE;
        status_register = call FlashHpl.read_status_register();
        //call FlashHpl.chip_erase();
        call FlashHpl.read(0, &readBuffer, len);
    }

    event void FlashHpl.read_done(void *rxBuffer, uint32_t rx_len) {
        for(i=0; i < rx_len; i++) {
            p->readBuffer[i] = readBuffer[i];
            p->statusRegister = status_register;
        }
        call SplitControl.start();
    }


    event void AMSend.sendDone(message_t *msg, error_t err) {
        call Leds.led0Toggle();
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
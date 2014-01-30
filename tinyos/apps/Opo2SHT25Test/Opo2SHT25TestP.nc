#include "Opo2SHT25Test.h"

module Opo2SHT25TestP {
	uses {
		interface Boot;
		interface Leds;
		interface AMSend;
		interface SplitControl;
		interface HplSHT25;
        interface Timer<TMilli> as SenseTimer;
        interface HplMsp430GeneralIO as I2CSwitch;
	}
}
implementation {
    int i = 0;
    message_t packet;
    opo_sht25_test_msg_t *p;
    uint16_t mTemp = 0;
    uint16_t mRH = 0;

    event void Boot.booted() {
        p = (opo_sht25_test_msg_t*) call AMSend.getPayload(&packet, sizeof(opo_sht25_test_msg_t));
        call I2CSwitch.makeOutput();
        call I2CSwitch.set();
        call HplSHT25.softReset();

    }

    event void HplSHT25.readTemperatureDone(uint16_t temp) {
    	mTemp = temp;
        call HplSHT25.readRH();
    }

    event void HplSHT25.readRHDone(uint16_t rh) {
        mRH = rh;
        call Leds.led0On();
        call I2CSwitch.clr();
        call SplitControl.start();
    }

    event void HplSHT25.softResetDone() {
        call SenseTimer.startOneShot(2000);
    }

    event void AMSend.sendDone(message_t *msg, error_t err) {

        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_sht25_test_msg_t));
    }

    event void SplitControl.startDone(error_t err) {
        p->temp = mTemp;
        p->rh = mRH;
        call Leds.led1On();
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_sht25_test_msg_t));
    }

    event void SenseTimer.fired() {
        call HplSHT25.readTemperature();
    }

    event void SplitControl.stopDone(error_t err) {}
}
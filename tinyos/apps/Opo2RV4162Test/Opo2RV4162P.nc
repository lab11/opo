#include "Opo2RV4162.h"
module Opo2RV4162P {
	uses {
		interface Boot;
		interface Leds;
		interface AMSend;
		interface SplitControl;
		interface HplRV4162 as RTC;
        interface Timer<TMilli> as SenseTimer;
        interface HplMsp430GeneralIO as I2CSwitch;
	}
}
implementation {

	uint8_t m_full_time[8];

	int i = 0;
	message_t packet;
	opo_rtc_msg_t *p;

	event void Boot.booted() {
		call I2CSwitch.makeOutput();
		call I2CSwitch.clr();

		p = (opo_rtc_msg_t *) call AMSend.getPayload(&packet, 8);

		m_full_time[0] = 0;
		m_full_time[1] = 0;
		m_full_time[2] = 4;
		m_full_time[3] = 8;
		m_full_time[4] = 3;
		m_full_time[5] = 12;
		m_full_time[6] = 3;
		m_full_time[7] = 14;

		call SplitControl.start();

	}

	event void AMSend.sendDone(message_t *msg, error_t err) {
        call Leds.led0Toggle();
        call SenseTimer.startOneShot(1000);
    }



    event void RTC.readFullTimeDone(error_t err, uint8_t* fullTime) {
    	for(i=0; i < 8; i++) {
    		p->full_time[i] = fullTime[i];
    	}

    	call I2CSwitch.clr();
    	call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_rtc_msg_t));
    }

    event void RTC.writeSTBitDone(error_t err) {
        call RTC.readFullTime();
    }

    event void RTC.setTimeDone(error_t err) {
    	call RTC.readFullTime();
    }

    event void RTC.resetTimeDone(error_t err) {
        call RTC.readFullTime();
    }

    event void SplitControl.startDone(error_t err) {
    	call Leds.led1On();
    	call I2CSwitch.set();
    	call RTC.setTime(m_full_time);
    }

    event void SplitControl.stopDone(error_t err) {

    }

    event void SenseTimer.fired() {
    	call I2CSwitch.set();
        call RTC.readFullTime();
    }
}
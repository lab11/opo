module OpoSleepP {
	uses {
		interface Boot;
		interface SplitControl as FlashControl;
		interface SplitControl as RfControl;
		interface HplRV4162;
		interface Timer<TMilli> as RtcTimer;
	}
}
implementation {

	event void Boot.booted() {
		call RfControl.stop();
	}

	event void RfControl.stopDone(error_t err) {
		call FlashControl.stop();
	}

	event void RfControl.startDone(error_t err) {
	}

	event void FlashControl.stopDone(error_t err) {
		//call RtcTimer.startOneShot(1000);
	}

	event void RtcTimer.fired() {
		call HplRV4162.writeSTBit();
	}

	event void FlashControl.startDone(error_t err) {}

	event void HplRV4162.readFullTimeDone(error_t err, uint8_t* fullTime) {}

    event void HplRV4162.writeSTBitDone(error_t err) {
    	call RtcTimer.startOneShot(1500);
    }

    event void HplRV4162.setTimeDone(error_t err) {}

    event void HplRV4162.resetTimeDone(error_t err) {}

}
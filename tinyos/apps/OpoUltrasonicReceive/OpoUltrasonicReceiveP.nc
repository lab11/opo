module OpoUltrasonicReceiveP {
	uses {
		interface HplMsp430GeneralIO as TxRxSwitch;
		interface HplMsp430GeneralIO as ULRxPin;
		interface HplMsp430GeneralIO as RxPin;
		interface Msp430Capture as ULCapture;
    	interface Msp430TimerControl as ULCapControl;
    	interface Leds;
    	interface Boot;
    	interface Timer<TMilli> as RxTimer;
	}
}
implementation {

	event void Boot.booted() {
		call ULRxPin.makeInput();
		call ULRxPin.selectModuleFunc();
		call ULCapControl.setControlAsCapture(1);
		call ULCapControl.enableEvents();
		call ULCapControl.clearPendingInterrupt();

		call ULCapture.setEdge(MSP430TIMER_CM_RISING);
		call TxRxSwitch.clr();

		call RxPin.makeOutput();
		call RxPin.clr();

		call RxTimer.startOneShot(3000);
	}

	async event void ULCapture.captured(uint16_t time) {
		call ULCapture.setEdge(MSP430TIMER_CM_NONE);
		call Leds.led0On();
		call RxTimer.startOneShot(2000);
	}

	event void RxTimer.fired() {
		call Leds.led0Off();
		call ULCapture.setEdge(MSP430TIMER_CM_RISING);
	}

}
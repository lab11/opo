#include "OpoUltrasonicPins.h"

/*
	Port number sotred in h file
*/
configuration OpoUltrasonicPinsC {
	provides {
		interface HplMsp430GeneralIO as TxRxSwitch;
		interface HplMsp430GeneralIO as ULRxPin;
		interface Msp430Capture as ULCapture;
    	interface Msp430TimerControl as ULCapControl;
	}

}
implementation {

	components HplMsp430GeneralIO as GpIO;

  	TxRxSwitch = GpIO.TxRxSwitch;
  	ULRxPin = GpIO.ULRxPin;

  	components Msp430TimerC;

  	ULCapture = Msp430TimerC.ULCap;
  	ULCapControl = Msp430TimerC.ULCapControl;

}
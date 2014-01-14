configuration OpoUltrasonicReceiveC {

}
implementation {
	components MainC, LedsC, OpoUltrasonicReceiveP;

	OpoUltrasonicReceiveP.Boot -> MainC.Boot;
	OpoUltrasonicReceiveP.Leds -> LedsC.Leds;

	components new TimerMilliC() as RxTimer;
	OpoUltrasonicReceiveP.RxTimer -> RxTimer;

	components HplMsp430GeneralIOC as GeneralIO;
	OpoUltrasonicReceiveP.RxPin -> GeneralIO.Port56;
	OpoUltrasonicReceiveP.TxRxSwitch -> GeneralIO.Port50;
	OpoUltrasonicReceiveP.ULRxPin -> GeneralIO.Port43;

	components Msp430TimerC;
	OpoUltrasonicReceiveP.ULCapture -> Msp430TimerC.CaptureB3;
	OpoUltrasonicReceiveP.ULCapControl -> Msp430TimerC.ControlB3;
}
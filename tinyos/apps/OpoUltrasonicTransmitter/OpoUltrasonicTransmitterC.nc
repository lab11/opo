configuration OpoUltrasonicTransmitterC {

}
implementation {
	components MainC, OpoUltrasonicTransmitterP, LedsC;

	OpoUltrasonicTransmitterP.Boot -> MainC.Boot;
	OpoUltrasonicTransmitterP.Leds -> LedsC.Leds;

	components HplMsp430GeneralIOC as GpIO;
	OpoUltrasonicTransmitterP.TxRxSwitch -> GpIO.Port50;

	components new TimerMilliC() as TxTimer;
	OpoUltrasonicTransmitterP.TxTimer -> TxTimer;
}
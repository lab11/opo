configuration HplSST25VF064C {
	provides {
		interface HplSST25VF064;
	}
}
implementation {
	components HplSST25VF064P;

	HplSST25VF064 = HplSST25VF064P;

	components BusyWaitMicroC;
	HplSST25VF064P.BusyWait -> BusyWaitMicroC;

	components LedsC;
	HplSST25VF064P.Leds -> LedsC.Leds;

	components new Msp430Spi1C() as FlashSpi;
	HplSST25VF064P.SpiResource -> FlashSpi.Resource;
	HplSST25VF064P.SpiByte -> FlashSpi.SpiByte;

	components HplMsp430GeneralIOC as GpIO;
	components new Msp430GpioC() as FlashPowerGateM;
  	components new Msp430GpioC() as FlashCSM;
  	components new Msp430GpioC() as ResetHoldPinM;
  	components new Msp430GpioC() as WriteProtectPinM;
  	FlashPowerGateM -> GpIO.Port64;
  	FlashCSM -> GpIO.Port44;
  	ResetHoldPinM -> GpIO.Port55;
  	WriteProtectPinM -> GpIO.Port54;

	HplSST25VF064P.FlashPowerGate -> FlashPowerGateM;
	HplSST25VF064P.FlashCS -> FlashCSM;
	HplSST25VF064P.ResetHoldPin -> ResetHoldPinM;
	HplSST25VF064P.WriteProtectPin -> WriteProtectPinM;

	components new TimerMilliC() as WaitTimer;
	HplSST25VF064P.WaitTimer -> WaitTimer;
}
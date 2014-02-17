configuration HplAt45dbC {
	provides {
		interface HplAt45db;
	}
}
implementation {
	components HplAt45dbP;

	HplAt45db = HplAt45dbP;

	components BusyWaitMicroC;
	HplAt45dbP.BusyWait -> BusyWaitMicroC;

	components LedsC;
	HplAt45dbP.Leds -> LedsC.Leds;

	components new Msp430Spi0C() as FlashSpi;
	HplAt45dbP.SpiResource -> FlashSpi.Resource;
	HplAt45dbP.SpiByte -> FlashSpi.SpiByte;
	HplAt45dbP.SpiPacket -> FlashSpi.SpiPacket;

	components HplMsp430GeneralIOC as GpIO;
  	components new Msp430GpioC() as FlashCSM;
  	components new Msp430GpioC() as ResetPinM;

  	FlashCSM -> GpIO.Port44;
  	ResetPinM -> GpIO.Port17;

	HplAt45dbP.FlashCS -> FlashCSM;
	HplAt45dbP.FlashReset -> ResetPinM;

	components new TimerMilliC() as WaitTimer;
	HplAt45dbP.WaitTimer -> WaitTimer;
}
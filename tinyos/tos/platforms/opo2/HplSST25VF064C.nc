configuration HplSST25VF064C {
	provides {
		interface HplSST25VF064;
	}
}
implementatoin {
	components HplSST25VF064P;

	components new Msp430Spi1C() as FlashSpi;
	HplSST25VF064P.SpiResource -> FlashSpi.Resource;
	HplSST25VF064P.SpiByte -> FlashSpi.SpiByte;

	components new MspGeneralIOC() as GpIO;
	HplSST25VF064P.FlashPowerGate -> GpIO.Port64;
	HplSST25VF064P.FlashCS -> GpIO.Port44;
	HplSST25VF064P.ResetHoldPin -> GpIO.Port55;




}
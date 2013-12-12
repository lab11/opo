configuration OpoADCTestC {

}
implementation {
	components MainC, LedsC, OpoADCTestP;
	OpoADCTestP.Boot -> MainC.Boot;
	OpoADCTestP.Leds -> LedsC.Leds;

	components new Msp430Adc12ClientC() as OpoADC;
  //components new Msp430Adc12ClientAutoDMAC() as OpoADC;
	OpoADCTestP.ReadSingleChannel -> OpoADC.Msp430Adc12SingleChannel;
 	OpoADCTestP.AdcResource -> OpoADC.Resource;

	components HplMsp430GeneralIOC as GpIO;
	OpoADCTestP.UCapGpIO -> GpIO.Port43;
	OpoADCTestP.SFDCapGpIO -> GpIO.Port41;
	OpoADCTestP.SFDLatch -> GpIO.Port52;
	OpoADCTestP.TxRxSel -> GpIO.Port50;
	OpoADCTestP.TxGate -> GpIO.Port53;
	OpoADCTestP.Adc0 -> GpIO.Port60;
  	OpoADCTestP.TimingLatch -> GpIO.Port17;

	components Msp430TimerC;
	OpoADCTestP.UltrasonicCapture -> Msp430TimerC.CaptureB3;
	OpoADCTestP.UCapControl -> Msp430TimerC.ControlB3;
	OpoADCTestP.SFDCapture -> Msp430TimerC.CaptureB1;
	OpoADCTestP.SFDCapControl -> Msp430TimerC.ControlB1;
	OpoADCTestP.TimerB -> Msp430TimerC.TimerB;

	components ActiveMessageC;
	components new AMSenderC(OPO_ADC_BASE) as OpoAdcRf;
	OpoADCTestP.RfControl -> ActiveMessageC.SplitControl;
	OpoADCTestP.AMSend -> OpoAdcRf.AMSend;

	components CC2420ControlC;
    OpoADCTestP.CC2420Config -> CC2420ControlC.CC2420Config;

	components new TimerMilliC() as RxTimer;
  	components new TimerMilliC() as RfTimer;
  	components new TimerMilliC() as AdcTimer;
	OpoADCTestP.RxTimer -> RxTimer;
  	OpoADCTestP.RfTimer -> RfTimer;
  	OpoADCTestP.AdcTimer -> AdcTimer;
}
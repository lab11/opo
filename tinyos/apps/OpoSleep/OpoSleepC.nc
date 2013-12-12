configuration OpoSleepC {

}
implementation {
	components MainC, OpoSleepP;
	OpoSleepP.Boot -> MainC.Boot;

	components At45dbPowerC, RV4162C;
	OpoSleepP.FlashControl -> At45dbPowerC.SplitControl;
	OpoSleepP.HplRV4162 -> RV4162C.HplRV4162;

	components ActiveMessageC;
	OpoSleepP.RfControl -> ActiveMessageC.SplitControl;

	components new TimerMilliC() as RtcTimer;
	OpoSleepP.RtcTimer -> RtcTimer;
}
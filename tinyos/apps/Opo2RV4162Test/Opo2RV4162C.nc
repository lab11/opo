#include "Opo2RV4162.h"

configuration Opo2RV4162C {

}
implementation {
	components Opo2RV4162P, MainC, LedsC;
    Opo2RV4162P.Boot -> MainC.Boot;
    Opo2RV4162P.Leds -> LedsC.Leds;

    components RV4162C;
	Opo2RV4162P.RTC -> RV4162C.HplRV4162;

    components ActiveMessageC;
    components new AMSenderC(OPO_RTC_TEST) as OpoRf;

    Opo2RV4162P.AMSend -> OpoRf.AMSend;
    Opo2RV4162P.SplitControl -> ActiveMessageC.SplitControl;

    components HplMsp430GeneralIOC as GpIO;
    Opo2RV4162P.I2CSwitch -> GpIO.Port65;

    components new TimerMilliC() as SenseTimer;
    Opo2RV4162P.SenseTimer -> SenseTimer;
}
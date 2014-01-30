#include "Opo2SHT25Test.h"

configuration Opo2SHT25TestC {

}
implementation {
	components Opo2SHT25TestP, MainC, LedsC;
    Opo2SHT25TestP.Boot -> MainC.Boot;
    Opo2SHT25TestP.Leds -> LedsC.Leds;

    components HplSHT25C;
    Opo2SHT25TestP.HplSHT25 -> HplSHT25C.HplSHT25;

    components ActiveMessageC;
    components new AMSenderC(OPO_SHT25_TEST) as OpoSHT25Transmit;

    Opo2SHT25TestP.AMSend -> OpoSHT25Transmit.AMSend;
    Opo2SHT25TestP.SplitControl -> ActiveMessageC.SplitControl;

    components HplMsp430GeneralIOC as GpIO;
    Opo2SHT25TestP.I2CSwitch -> GpIO.Port65;

    components new TimerMilliC() as SenseTimer;
    Opo2SHT25TestP.SenseTimer -> SenseTimer;
}
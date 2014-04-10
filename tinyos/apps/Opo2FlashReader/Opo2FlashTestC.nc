#include "Opo2FlashTest.h"
configuration Opo2FlashTestC {

}
implementation {
	components Opo2FlashTestP, MainC, LedsC;
    Opo2FlashTestP.Boot -> MainC.Boot;
    Opo2FlashTestP.Leds -> LedsC.Leds;

    components HplSST25VF064C;
    Opo2FlashTestP.FlashHpl -> HplSST25VF064C.HplSST25VF064;

    components ActiveMessageC;
    components new AMSenderC(OPO_FLASH_STORE_BASE) as OpoRfTransmit;

    Opo2FlashTestP.AMSend -> OpoRfTransmit.AMSend;
    Opo2FlashTestP.SplitControl -> ActiveMessageC.SplitControl;

    components new TimerMilliC() as FlashTimer;
    Opo2FlashTestP.FlashTimer -> FlashTimer;
}
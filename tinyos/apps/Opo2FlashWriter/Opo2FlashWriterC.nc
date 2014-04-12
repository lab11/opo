#include "Opo2FlashWriter.h"
configuration Opo2FlashWriterC {

}
implementation {
	components Opo2FlashWriterP, MainC, LedsC;
    Opo2FlashWriterP.Boot -> MainC.Boot;
    Opo2FlashWriterP.Leds -> LedsC.Leds;

    components HplSST25VF064C;
    Opo2FlashWriterP.FlashHpl -> HplSST25VF064C.HplSST25VF064;

    components ActiveMessageC;
    components new AMSenderC(OPO_FLASH_STORE_BASE) as OpoRfTransmit;

    Opo2FlashWriterP.AMSend -> OpoRfTransmit.AMSend;
    Opo2FlashWriterP.SplitControl -> ActiveMessageC.SplitControl;

    components new TimerMilliC() as FlashTimer;
    Opo2FlashWriterP.FlashTimer -> FlashTimer;
}

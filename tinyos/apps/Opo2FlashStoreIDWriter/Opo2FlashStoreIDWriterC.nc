#include "Opo2FlashStoreIDWriter.h"

configuration Opo2FlashStoreIDWriterC {}

implementation {

    components Opo2FlashStoreIDWriterP, MainC, LedsC;
    Opo2FlashStoreIDWriterP.Boot -> MainC.Boot;
    Opo2FlashStoreIDWriterP.Leds -> LedsC.Leds;

    components ActiveMessageC;
    components new AMSenderC(OPO_ID_WRITER_BASE) as OpoRfSend;

    Opo2FlashStoreIDWriterP.AMSend -> OpoRfSend.AMSend;
    Opo2FlashStoreIDWriterP.RfControl -> ActiveMessageC.SplitControl;

    components HplSST25VF064C;
    Opo2FlashStoreIDWriterP.FlashHpl -> HplSST25VF064C.HplSST25VF064;

    components new TimerMilliC() as SendTimer;
    Opo2FlashStoreIDWriterP.SendTimer -> SendTimer;
}
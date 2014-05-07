#include "Opo2FlashStore.h"

configuration Opo2FlashStoreC {}

implementation {
    components Opo2FlashStoreP, MainC, LedsC;
    Opo2FlashStoreP.Boot -> MainC.Boot;
    Opo2FlashStoreP.Leds -> LedsC.Leds;

    components RandomMtC;
    Opo2FlashStoreP.RandomMt -> RandomMtC.RandomMt;

    components ActiveMessageC;
    components new AMSenderC(OPO_FLASH_STORE_BASE) as OpoBaseSend;

    Opo2FlashStoreP.OpoBaseSend -> OpoBaseSend.AMSend;
    Opo2FlashStoreP.RadControl -> ActiveMessageC.SplitControl;

    components HplMsp430GeneralIOC as GpIO;
    Opo2FlashStoreP.I2CSwitch -> GpIO.Port65;

    components HplSST25VF064C;
    Opo2FlashStoreP.FlashHpl -> HplSST25VF064C.HplSST25VF064;

    components new TimerMilliC() as LedTimer;
    Opo2FlashStoreP.LedTimer -> LedTimer;
}
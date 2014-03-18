#include "Opo2FlashStore.h"

configuration Opo2FlashStoreC {}

implementation {
    components Opo2FlashStoreP, MainC, LedsC;
    Opo2FlashStoreP.Boot -> MainC.Boot;
    Opo2FlashStoreP.Leds -> LedsC.Leds;

    components RandomMtC;
    Opo2FlashStoreP.RandomMt -> RandomMtC.RandomMt;

    components OpoC;
    Opo2FlashStoreP.Opo -> OpoC.Opo;

    components ActiveMessageC;
    components CC2420PacketC;
    components new AMSenderC(OPO_FLASH_STORE) as OpoRfSend;
    components new AMReceiverC(OPO_FLASH_STORE) as OpoRfReceive;

    OpoC.AMSend -> OpoRfSend.AMSend;
    OpoC.AMReceive -> OpoRfReceive.Receive;
    OpoC.RfControl -> ActiveMessageC.SplitControl;
    Opo2FlashStoreP.Packet -> ActiveMessageC.Packet;
    Opo2FlashStoreP.PacketAcks -> ActiveMessageC.PacketAcknowledgements;
    Opo2FlashStoreP.CC2420Packet -> CC2420PacketC.CC2420Packet;

    components HplMsp430GeneralIOC as GpIO;
    Opo2FlashStoreP.I2CSwitch -> GpIO.Port65;

    components CC2420ControlC;
    Opo2FlashStoreP.CC2420Config -> CC2420ControlC.CC2420Config;

    components HplSST25VF064C;
    Opo2FlashStoreP.FlashHpl -> HplSST25VF064C.HplSST25VF064;

    components RV4162C;
    Opo2FlashStoreP.HplRV4162 -> RV4162C.HplRV4162;

    components new TimerMilliC() as TxTimer;
    components new TimerMilliC() as RxTimer;
    components new TimerMilliC() as LedTimer;
    Opo2FlashStoreP.TxTimer -> TxTimer;
    Opo2FlashStoreP.RxTimer -> RxTimer;
    Opo2FlashStoreP.LedTimer -> LedTimer;
}
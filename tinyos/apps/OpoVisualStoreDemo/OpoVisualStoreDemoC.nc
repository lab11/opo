#include "OpoVisualStoreDemo.h"
#include "StorageVolumes.h"

configuration OpoVisualStoreDemoC {}

implementation {
#ifdef OPO_DEBUG
    components PrintfC, SerialStartC;
#endif

    components OpoVisualStoreDemoP, MainC, LedsC;
    OpoVisualStoreDemoP.Boot -> MainC.Boot;
    OpoVisualStoreDemoP.Leds -> LedsC.Leds;

    components Ds2411C, RandomC;
    OpoVisualStoreDemoP.IdReader -> Ds2411C;
    OpoVisualStoreDemoP.Random -> RandomC;

    components new BlockStorageC(VOLUME_BLOCKSTORE);
    OpoVisualStoreDemoP.BlockWrite -> BlockStorageC.BlockWrite;

    components OpoC;
    OpoVisualStoreDemoP.Opo -> OpoC.Opo;

    components ActiveMessageC;
    components new AMSenderC(OPO_VISUAL_DEMO) as OpoRfSend;
    components new AMReceiverC(OPO_VISUAL_DEMO) as OpoRfReceive;
    components new AMSenderC(OPO_VISUAL_BASE) as BaseSend;

    OpoVisualStoreDemoP.Packet -> ActiveMessageC.Packet;
    OpoC.AMSend -> OpoRfSend.AMSend;
    OpoC.AMReceive -> OpoRfReceive.Receive;
    OpoC.RfControl -> ActiveMessageC.SplitControl;
    OpoVisualStoreDemoP.RfControl -> ActiveMessageC.SplitControl;
    OpoVisualStoreDemoP.BaseSend -> BaseSend.AMSend;
    OpoVisualStoreDemoP.PacketAcks -> ActiveMessageC.PacketAcknowledgements;

    components CC2420ControlC;
    OpoVisualStoreDemoP.CC2420Config -> CC2420ControlC.CC2420Config;

    components At45dbPowerC;
    OpoVisualStoreDemoP.FlashPower -> At45dbPowerC.SplitControl;

    components RV4162C;
    OpoVisualStoreDemoP.HplRV4162 -> RV4162C.HplRV4162;

    components new TimerMilliC() as TxTimer;
    components new TimerMilliC() as RxTimer;
    components new TimerMilliC() as RTCTimer;
    OpoVisualStoreDemoP.TxTimer -> TxTimer;
    OpoVisualStoreDemoP.RxTimer -> RxTimer;
    OpoVisualStoreDemoP.RTCTimer -> RTCTimer;

}
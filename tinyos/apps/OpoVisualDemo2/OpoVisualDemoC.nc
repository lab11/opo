#include "OpoVisualDemo.h"

configuration OpoVisualDemoC {}

implementation {
#ifdef OPO_DEBUG
    components PrintfC, SerialStartC;
#endif

    components OpoVisualDemoP, MainC, LedsC;
    OpoVisualDemoP.Boot -> MainC.Boot;
    OpoVisualDemoP.Leds -> LedsC.Leds;

    components Ds2411C, RandomC;
    OpoVisualDemoP.IdReader -> Ds2411C;
    OpoVisualDemoP.Random -> RandomC;

    components OpoC;
    OpoVisualDemoP.Opo -> OpoC.Opo;

    components ActiveMessageC;
    components new AMSenderC(OPO_VISUAL_DEMO) as OpoRfSend;
    components new AMReceiverC(OPO_VISUAL_DEMO) as OpoRfReceive;
    components new AMSenderC(OPO_VISUAL_BASE) as BaseSend;

    OpoVisualDemoP.Packet -> ActiveMessageC.Packet;
    OpoC.AMSend -> OpoRfSend.AMSend;
    OpoC.AMReceive -> OpoRfReceive.Receive;
    OpoC.RfControl -> ActiveMessageC.SplitControl;
    OpoVisualDemoP.RfControl -> ActiveMessageC.SplitControl;
    OpoVisualDemoP.BaseSend -> BaseSend.AMSend;
    OpoVisualDemoP.PacketAcks -> ActiveMessageC.PacketAcknowledgements;

    components CC2420ControlC;
    OpoVisualDemoP.CC2420Config -> CC2420ControlC.CC2420Config;

    components At45dbPowerC;
    OpoVisualDemoP.FlashPower -> At45dbPowerC.SplitControl;

    components RV4162C;
    OpoVisualDemoP.HplRV4162 -> RV4162C.HplRV4162;

    components new TimerMilliC() as TxTimer;
    components new TimerMilliC() as RxTimer;
    components new TimerMilliC() as RTCTimer;
    OpoVisualDemoP.TxTimer -> TxTimer;
    OpoVisualDemoP.RxTimer -> RxTimer;
    OpoVisualDemoP.RTCTimer -> RTCTimer;

}
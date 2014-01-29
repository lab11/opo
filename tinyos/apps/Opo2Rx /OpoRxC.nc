#include "OpoRx.h"

configuration OpoRxC {}

implementation {
    components MainC, LedsC, OpoRxP;
    OpoRxP.Boot -> MainC.Boot;
    OpoRxP.Leds -> LedsC.Leds;

    components Ds2411C, RandomC;
    OpoRxP.IdReader -> Ds2411C;
    OpoRxP.Random -> RandomC;

    components OpoC;
    OpoRxP.Opo -> OpoC.Opo;

    components ActiveMessageC;
    components new AMSenderC(OPO_RX_TX) as OpoRfSend;
    components new AMReceiverC(OPO_RX_TX) as OpoRfReceive;
    components new AMSenderC(OPO_RX_BASE) as BaseSend;

    OpoRxP.Packet -> ActiveMessageC.Packet;
    OpoRxP.BaseSend -> BaseSend;
    OpoRxP.RfControl -> ActiveMessageC.SplitControl;
    OpoC.AMSend -> OpoRfSend;
    OpoC.AMReceive -> OpoRfReceive;
    OpoC.RfControl -> ActiveMessageC.SplitControl;

    components CC2420ControlC;
    OpoRxP.CC2420Config -> CC2420ControlC.CC2420Config;

    components At45dbPowerC;
    OpoRxP.At45dbPower -> At45dbPowerC;

    components new TimerMilliC() as RxTimer;
    components new TimerMilliC() as BaseTimer;
    OpoRxP.RxTimer -> RxTimer;
    OpoRxP.BaseTimer -> BaseTimer;
}
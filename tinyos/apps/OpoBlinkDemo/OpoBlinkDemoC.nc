#include "OpoBlinkDemo.h"

configuration OpoBlinkDemoC {}

implementation {
#ifdef OPO_DEBUG
    components PrintfC, SerialStartC;
#endif

    components OpoBlinkDemoP, MainC, LedsC;
    OpoBlinkDemoP.Boot -> MainC.Boot;
    OpoBlinkDemoP.Leds -> LedsC.Leds;

    components Ds2411C, RandomC;
    OpoBlinkDemoP.IdReader -> Ds2411C;
    OpoBlinkDemoP.Random -> RandomC;

    components OpoC;
    OpoBlinkDemoP.Opo -> OpoC.Opo;

    components ActiveMessageC;
    components new AMSenderC(OPO_BLINK) as OpoRfSend;
    components new AMReceiverC(OPO_BLINK) as OpoRfReceive;

    OpoBlinkDemoP.Packet -> ActiveMessageC.Packet;
    OpoC.AMSend -> OpoRfSend.AMSend;
    OpoC.AMReceive -> OpoRfReceive.Receive;
    OpoC.RfControl -> ActiveMessageC.SplitControl;

    components At45dbPowerC;
    OpoBlinkDemoP.FlashPower -> At45dbPowerC.SplitControl;

    components new TimerMilliC() as TxTimer;
    components new TimerMilliC() as RxTimer;
    components new TimerMilliC() as LedsTimer;
    OpoBlinkDemoP.TxTimer -> TxTimer;
    OpoBlinkDemoP.RxTimer -> RxTimer;
    OpoBlinkDemoP.LedsTimer -> LedsTimer;

}
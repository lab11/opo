#include "OpoSimpleDemo.h"

configuration OpoSimpleDemoC {}

implementation {
    components MainC, LedsC, OpoSimpleDemoP, RandomC;
    OpoSimpleDemoP.Boot -> MainC.Boot;
    OpoSimpleDemoP.Leds -> LedsC.Leds;
    OpoSimpleDemoP.Random -> RandomC;

    components Ds2411C;
    OpoSimpleDemoP.IdReader -> Ds2411C;

    components At45dbPowerC;
    OpoSimpleDemoP.FlashPower -> At45dbPowerC;

    components OpoC;
    OpoSimpleDemoP.Opo -> OpoC.Opo;

    components ActiveMessageC;
    components new AMSenderC(OPO_SIMPLE) as OpoRfSend;
    components new AMSenderC(PREP) as PrepSend;
    components new AMReceiverC(OPO_SIMPLE) as OpoRfReceive;

    OpoC.RfControl -> ActiveMessageC.SplitControl;
    OpoC.AMSend -> OpoRfSend;
    OpoC.AMReceive -> OpoRfReceive;
    OpoSimpleDemoP.PrepSend -> PrepSend;

    components new TimerMilliC() as TxTimer;
    //components new TimerMilliC() as RxTimer;
    //components new TimerMilliC() as LedTimer;
    OpoSimpleDemoP.TxTimer -> TxTimer;
    //OpoSimpleDemoP.RxTimer -> RxTimer;
    //OpoSimpleDemoP.LedTimer -> LedTimer;
}
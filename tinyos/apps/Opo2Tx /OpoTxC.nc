#include "OpoTx.h"

configuration OpoTxC {}

implementation {
    components OpoTxP, MainC, LedsC;
    OpoTxP.Boot -> MainC.Boot;
    OpoTxP.Leds -> LedsC.Leds;

    components OpoC;
    OpoTxP.Opo -> OpoC.Opo;

    components ActiveMessageC;
    components new AMSenderC(OPO_RX_TX) as OpoRfSend;
    components new AMSenderC(PREP) as PrepSend;
    components new AMReceiverC(OPO_RX_TX) as OpoRfReceive;

    OpoTxP.PrepSend -> PrepSend;
    OpoC.AMSend -> OpoRfSend;
    OpoC.AMReceive -> OpoRfReceive;
    OpoC.RfControl -> ActiveMessageC.SplitControl;

    components HplSST25VF064C;
    OpoTxP.Flash -> HplSST25VF064C.HplSST25VF064;

    components new TimerMilliC() as OpoTxTimer;
    OpoTxP.OpoTxTimer -> OpoTxTimer;
}
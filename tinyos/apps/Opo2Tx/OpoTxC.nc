#include "OpoTx.h"

configuration OpoTxC {}

implementation {
    components OpoTxP, MainC, LedsC;
    OpoTxP.Boot -> MainC.Boot;
    OpoTxP.Leds -> LedsC.Leds;

    components OpoC;
    OpoTxP.Opo -> OpoC.Opo;

    components ActiveMessageC;
    components new AMSenderC(OPO_FLASH_STORE) as OpoRfSend;
    components new AMSenderC(PREP) as PrepSend;
    components new AMReceiverC(OPO_FLASH_STORE) as OpoRfReceive;

    OpoTxP.PrepSend -> PrepSend;
    OpoC.AMSend -> OpoRfSend;
    OpoC.AMReceive -> OpoRfReceive;
    OpoC.RfControl -> ActiveMessageC.SplitControl;

    components HplMsp430GeneralIOC as GpIO;
    OpoTxP.I2CSwitch -> GpIO.Port65;
    OpoC.I2CSwitch -> GpIO.Port65;

    components RV4162C;
    OpoTxP.RTCHpl -> RV4162C.HplRV4162;

    components new TimerMilliC() as OpoTxTimer;
    OpoTxP.OpoTxTimer -> OpoTxTimer;
}
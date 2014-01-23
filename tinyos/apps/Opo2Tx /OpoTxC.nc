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

    components HplMsp430GeneralIOC as GpIO;
    OpoTxP.AMP3_ADC -> GpIO.AMP3_ADC;
    OpoTxP.SFDIntGpio -> GpIO.SFDPIN;

    components new TimerMilliC() as TxTimer;
    components new TimerMilliC() as LedsTimer;
    OpoTxP.TxTimer -> TxTimer;
    OpoTxP.LedsTimer -> LedsTimer;
}
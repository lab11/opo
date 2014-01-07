#include "OpoRfTransmitter.h"

configuration OpoRfTransmitterC {}

implementation {
    components OpoRfTransmitterP, MainC, LedsC;
    OpoRfTransmitterP.Boot -> MainC.Boot;
    OpoRfTransmitterP.Leds -> LedsC.Leds;

    components ActiveMessageC;
    components new AMSenderC(OPO_RF_TEST) as OpoRfTransmit;

    OpoRfTransmitterP.AMSend -> OpoRfTransmit.AMSend;
    OpoRfTransmitterP.SplitControl -> ActiveMessageC.SplitControl;

    components new TimerMilliC() as RfSendTimer;
    OpoRfTransmitterP.RfSendTimer -> RfSendTimer;
}
#include "OpoRfReceiver.h"

configuration OpoRfReceiverC {}

implementation {
    components OpoRfReceiverP, MainC, LedsC;
    OpoRfReceiverP.Boot -> MainC.Boot;
    OpoRfReceiverP.Leds -> LedsC.Leds;

    components ActiveMessageC;
    components new AMReceiverC(OPO_RF_TEST) as OpoRfReceive;

    OpoRfReceiverP.Receive -> OpoRfReceive.Receive;
    OpoRfReceiverP.SplitControl -> ActiveMessageC.SplitControl;
}
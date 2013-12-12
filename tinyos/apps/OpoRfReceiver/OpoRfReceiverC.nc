#include "printf.h"
#include "OpoRfReceiver.h"

configuration OpoRfReceiverC {}

implementation {
    components OpoRfReceiverP, MainC, LedsC, PrintfC, SerialStartC;
    OpoRfReceiverP.Boot -> MainC.Boot;
    OpoRfReceiverP.Leds -> LedsC.Leds;

    components ActiveMessageC;
    components new AMReceiverC(OPO) as OpoRfReceive;

    OpoRfReceiverP.Receive -> OpoRfReceive.Receive;
    OpoRfReceiverP.SplitControl -> ActiveMessageC.SplitControl;
}
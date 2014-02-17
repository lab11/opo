#include "OpoFlashStoreIDWriter.h"

configuration OpoFlashStoreIDWriterC {}

implementation {

    components OpoFlashStoreIDWriterP, MainC, LedsC;
    OpoFlashStoreIDWriterP.Boot -> MainC.Boot;
    OpoFlashStoreIDWriterP.Leds -> LedsC.Leds;

    components ActiveMessageC;
    components new AMSenderC(OPO_ID_WRITER_BASE) as OpoRfSend;

    OpoFlashStoreIDWriterP.AMSend -> OpoRfSend.AMSend;
    OpoFlashStoreIDWriterP.RfControl -> ActiveMessageC.SplitControl;

    components HplAt45dbC;
    OpoFlashStoreIDWriterP.HplAt45db -> HplAt45dbC.HplAt45db;

    components new TimerMilliC() as SendTimer;
    OpoFlashStoreIDWriterP.SendTimer -> SendTimer;
}
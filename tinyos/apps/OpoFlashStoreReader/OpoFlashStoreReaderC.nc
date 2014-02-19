#include "OpoFlashStoreReader.h"

configuration OpoFlashStoreReaderC {}

implementation {

    components OpoFlashStoreReaderP, MainC, LedsC;
    OpoFlashStoreReaderP.Boot -> MainC.Boot;
    OpoFlashStoreReaderP.Leds -> LedsC.Leds;

    components ActiveMessageC;
    components new AMSenderC(OPO_FLASH_READER_BASE) as OpoRfSend;

    OpoFlashStoreReaderP.AMSend -> OpoRfSend.AMSend;
    OpoFlashStoreReaderP.RfControl -> ActiveMessageC.SplitControl;

    components HplAt45dbC;
    OpoFlashStoreReaderP.HplAt45db -> HplAt45dbC.HplAt45db;

    components new TimerMilliC() as ReadTimer;
    OpoFlashStoreReaderP.ReadTimer -> ReadTimer;

}
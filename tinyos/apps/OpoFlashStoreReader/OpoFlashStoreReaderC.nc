#include "OpoFlashStoreReader.h"
#include "StorageVolumes.h"

configuration OpoFlashStoreReaderC {}

implementation {

    components OpoFlashStoreReaderP, MainC, LedsC;
    OpoFlashStoreReaderP.Boot -> MainC.Boot;
    OpoFlashStoreReaderP.Leds -> LedsC.Leds;

    components ActiveMessageC;
    components new AMSenderC(OPO_FLASH_READER_BASE) as OpoRfSend;

    OpoFlashStoreReaderP.AMSend -> OpoRfSend.AMSend;
    OpoFlashStoreReaderP.RfControl -> ActiveMessageC.SplitControl;

    components At45dbPowerC;
    OpoFlashStoreReaderP.FlashPower -> At45dbPowerC.SplitControl;

    components new BlockStorageC(VOLUME_BLOCKSTORE);
    OpoFlashStoreReaderP.BlockRead -> BlockStorageC.BlockRead;

    components new TimerMilliC() as ReadTimer;
    OpoFlashStoreReaderP.ReadTimer -> ReadTimer;

}
#include "OpoFlashStoreIDWriter.h"
#include "StorageVolumes.h"

configuration OpoFlashStoreIDWriterC {}

implementation {
    components OpoFlashStoreIDWriterP, MainC, LedsC;
    OpoFlashStoreIDWriterP.Boot -> MainC.Boot;
    OpoFlashStoreIDWriterP.Leds -> LedsC.Leds;

    components ActiveMessageC;
    components new AMSenderC(OPO_ID_WRITER_BASE) as OpoRfSend;

    OpoFlashStoreIDWriterP.AMSend -> OpoRfSend.AMSend;
    OpoFlashStoreIDWriterP.RfControl -> ActiveMessageC.SplitControl;

    components At45dbPowerC;
    OpoFlashStoreIDWriterP.FlashPower -> At45dbPowerC.SplitControl;

    components new BlockStorageC(VOLUME_BLOCKSTORE);
    OpoFlashStoreIDWriterP.BlockWrite -> BlockStorageC.BlockWrite;
    OpoFlashStoreIDWriterP.BlockRead -> BlockStorageC.BlockRead;

    components new TimerMilliC() as SendTimer;
    OpoFlashStoreIDWriterP.SendTimer -> SendTimer;
}
#include "OpoFlashStoreEraser.h"
#include "StorageVolumes.h"

configuration OpoFlashStoreEraserC {}

implementation {

    components OpoFlashStoreEraserP, MainC, LedsC;
    OpoFlashStoreEraserP.Boot -> MainC.Boot;
    OpoFlashStoreEraserP.Leds -> LedsC.Leds;

    components At45dbPowerC;
    OpoFlashStoreEraserP.FlashPower -> At45dbPowerC.SplitControl;

    components new BlockStorageC(VOLUME_BLOCKSTORE);
    OpoFlashStoreEraserP.BlockWrite -> BlockStorageC.BlockWrite;
}
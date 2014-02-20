#include "OpoFlashStoreEraser.h"

configuration OpoFlashStoreEraserC {}

implementation {

    components OpoFlashStoreEraserP, MainC, LedsC;
    OpoFlashStoreEraserP.Boot -> MainC.Boot;
    OpoFlashStoreEraserP.Leds -> LedsC.Leds;

    components HplAt45dbC;
    OpoFlashStoreEraserP.HplAt45db -> HplAt45dbC.HplAt45db;
}
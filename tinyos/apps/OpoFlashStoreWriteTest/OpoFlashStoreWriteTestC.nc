#include "OpoFlashStoreWriteTest.h"

configuration OpoFlashStoreWriteTestC {}

implementation {

    components OpoFlashStoreWriteTestP, MainC, LedsC;
    OpoFlashStoreWriteTestP.Boot -> MainC.Boot;
    OpoFlashStoreWriteTestP.Leds -> LedsC.Leds;

    components HplAt45dbC;
    OpoFlashStoreWriteTestP.HplAt45db -> HplAt45dbC.HplAt45db;

    components new TimerMilliC() as WaitTimer;
    OpoFlashStoreWriteTestP.WaitTimer -> WaitTimer;

}
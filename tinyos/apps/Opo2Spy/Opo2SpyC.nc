#include "Opo2Spy.h"

configuration Opo2SpyC {}

implementation {
  components MainC, LedsC, SerialStartC, PrintfC, Opo2SpyP;
  Opo2SpyP.Boot -> MainC.Boot;
  Opo2SpyP.Leds -> LedsC;

  components ActiveMessageC;
  components new AMReceiverC(OPO_FLASH_STORE) as FlashStoreNodeReceive;

  Opo2SpyP.FlashStoreNodeReceive -> FlashStoreNodeReceive;

  Opo2SpyP.RfControl -> ActiveMessageC.SplitControl;
  Opo2SpyP.AMPacket -> ActiveMessageC.AMPacket;

}

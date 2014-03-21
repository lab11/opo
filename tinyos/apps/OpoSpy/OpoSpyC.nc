#include "OpoSpy.h"

configuration OpoSpyC {}

implementation {
  components MainC, LedsC, SerialStartC, PrintfC, OpoSpyP;
  OpoSpyP.Boot -> MainC.Boot;
  OpoSpyP.Leds -> LedsC;

  components ActiveMessageC;
  components new AMReceiverC(OPO_FLASH_STORE) as FlashStoreNodeReceive;

  OpoSpyP.FlashStoreNodeReceive -> FlashStoreNodeReceive;

  OpoSpyP.RfControl -> ActiveMessageC.SplitControl;
  OpoSpyP.AMPacket -> ActiveMessageC.AMPacket;

  components new LinuxUdpSocketC();
  OpoSpyP.LinuxUdpSocket -> LinuxUdpSocketC.LinuxUdpSocket;
}

#include "OpoNodeBase.h"

configuration OpoNodeBaseC {}

implementation {
  components MainC, LedsC, OpoNodeBaseP;
  OpoNodeBaseP.Boot -> MainC.Boot;
  OpoNodeBaseP.Leds -> LedsC;

  components ActiveMessageC;
  components new AMReceiverC(OPO_RX_BASE) as RxReceive;

  OpoNodeBaseP.RxReceive -> RxReceive;
  OpoNodeBaseP.RfControl -> ActiveMessageC.SplitControl;

  components PlatformSerialC;
  OpoNodeBaseP.UartStream -> PlatformSerialC.UartStream;
  OpoNodeBaseP.UartControl -> PlatformSerialC;
}

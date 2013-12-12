#include "OpoBase.h"

configuration OpoBaseC {}

implementation {
  components MainC, LedsC, SerialStartC, PrintfC, OpoBaseP;
  OpoBaseP.Boot -> MainC.Boot;
  OpoBaseP.Leds -> LedsC;

  components ActiveMessageC;
  components new AMReceiverC(OPO_DATA) as OpoReceive;
  components new AMReceiverC(OPO_BLINK) as BlinkReceive;
  components new AMReceiverC(OPO_VISUAL_BASE) as VisualReceive;
  components new AMReceiverC(OPO_RX_BASE) as RxReceive;
  components new AMReceiverC(OPO_ADC_BASE) as ADCReceive;

  OpoBaseP.OpoReceive -> OpoReceive;
  OpoBaseP.BlinkReceive -> BlinkReceive;
  OpoBaseP.VisualReceive -> VisualReceive;
  OpoBaseP.RxReceive -> RxReceive;
  OpoBaseP.ADCReceive -> ADCReceive;
  OpoBaseP.RfControl -> ActiveMessageC.SplitControl;
  OpoBaseP.AMPacket -> ActiveMessageC.AMPacket;
}

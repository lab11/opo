#include "Opo.h"

configuration OpoC {
  provides {
    interface Opo;
  }
  uses {
    interface AMSend;
    interface SplitControl as RfControl;
    interface Receive as AMReceive;
  }
}

implementation {
  #ifdef OPO_DEBUG
  components SerialStartC, PrintfC;
  #endif

  Opo = OpoP.Opo;

  components OpoP, MainC, LedsC;
  OpoP.Leds -> LedsC.Leds;

  OpoP.Receive = AMReceive;
  OpoP.AMSend = AMSend;
  OpoP.RfControl = RfControl;

  components HplMsp430GeneralIOC as GpIO;
  OpoP.UCapGpIO -> GpIO.U_CAP_PIN;
  OpoP.SFDCapGpIO -> GpIO.SFD_PIN;
  OpoP.SFDLatch -> GpIO.SFD_LATCH;
  OpoP.TxRxSel -> GpIO.TXRX_SEL;
  OpoP.TxGate -> GpIO.TX_GATE;

  components Msp430TimerC;
  OpoP.SFDCapture -> Msp430TimerC.SFD_CAPTURE;
  OpoP.SFDCapControl -> Msp430TimerC.SFD_CAPTURE_CONTROL;
  OpoP.UltrasonicCapture -> Msp430TimerC.U_CAPTURE;
  OpoP.UCapControl -> Msp430TimerC.U_CAPTURE_CONTROL;
  OpoP.TimerB -> Msp430TimerC.TimerB;

  components new TimerMilliC() as TxTimer;
  components new TimerMilliC() as RxTimer;
  OpoP.TxTimer -> TxTimer;
  OpoP.RxTimer -> RxTimer;

}

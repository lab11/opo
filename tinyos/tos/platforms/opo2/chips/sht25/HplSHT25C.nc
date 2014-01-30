#include "sht25.h"

configuration HplSHT25C {
    provides {
      interface HplSHT25;
    }
}
implementation {
    components HplSHT25P;
    HplSHT25 = HplSHT25P.HplSHT25;

    components LedsC;
    HplSHT25P.Leds -> LedsC.Leds;

    components new Msp430I2CC() as I2CA;
    HplSHT25P.I2CPacket -> I2CA.I2CBasicAddr;
    HplSHT25P.I2CResource -> I2CA.Resource;

    components new TimerMilliC() as WaitTimer;
    HplSHT25P.WaitTimer -> WaitTimer;
}
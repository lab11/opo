#include "Opo2FlashReader.h"
configuration Opo2FlashReaderC {

}
implementation {
	components Opo2FlashReaderP, MainC, LedsC;
    Opo2FlashReaderP.Boot -> MainC.Boot;
    Opo2FlashReaderP.Leds -> LedsC.Leds;

    components HplSST25VF064C;
    Opo2FlashReaderP.FlashHpl -> HplSST25VF064C.HplSST25VF064;

    components ActiveMessageC;
    components new AMSenderC(OPO_RX_BASE) as OpoRfTransmit;

    Opo2FlashReaderP.AMSend -> OpoRfTransmit.AMSend;
    Opo2FlashReaderP.SplitControl -> ActiveMessageC.SplitControl;

}
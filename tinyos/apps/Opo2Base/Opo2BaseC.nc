#include "Opo2Base.h"

configuration Opo2BaseC {}

implementation {
  components MainC, LedsC, SerialStartC, PrintfC, Opo2BaseP;
  Opo2BaseP.Boot -> MainC.Boot;
  Opo2BaseP.Leds -> LedsC;

  components ActiveMessageC;
  components new AMReceiverC(OPO_DATA) as OpoReceive;
  components new AMReceiverC(OPO_BLINK) as BlinkReceive;
  components new AMReceiverC(OPO_VISUAL_BASE) as VisualReceive;
  components new AMReceiverC(OPO_RX_BASE) as RxReceive;
  components new AMReceiverC(OPO_ADC_BASE) as ADCReceive;
  components new AMReceiverC(OPO_FLASH_TEST) as FlashReceive;
  components new AMReceiverC(OPO_SHT25_TEST) as SHT25Receive;
  components new AMReceiverC(OPO_RTC_TEST) as RTCReceive;
  components new AMReceiverC(OPO_ID_WRITER_BASE) as IDReceive;
  components new AMReceiverC(OPO_FLASH_READER_BASE) as FlashReaderReceive;
  components new AMReceiverC(OPO_FLASH_READER_TEST) as FlashReaderTestReceive;
  components new AMReceiverC(OPO_FLASH_STORE_BASE) as FlashStoreBaseReceive;

  Opo2BaseP.OpoReceive -> OpoReceive;
  Opo2BaseP.BlinkReceive -> BlinkReceive;
  Opo2BaseP.VisualReceive -> VisualReceive;
  Opo2BaseP.RxReceive -> RxReceive;
  Opo2BaseP.ADCReceive -> ADCReceive;
  Opo2BaseP.FlashReceive -> FlashReceive;
  Opo2BaseP.SHT25Receive -> SHT25Receive;
  Opo2BaseP.RTCReceive -> RTCReceive;
  Opo2BaseP.IDReceive -> IDReceive;
  Opo2BaseP.FlashReaderReceive -> FlashReaderReceive;
  Opo2BaseP.FlashReaderTestReceive -> FlashReaderTestReceive;
  Opo2BaseP.FlashStoreBaseReceive -> FlashStoreBaseReceive;

  Opo2BaseP.RfControl -> ActiveMessageC.SplitControl;
  Opo2BaseP.AMPacket -> ActiveMessageC.AMPacket;

}

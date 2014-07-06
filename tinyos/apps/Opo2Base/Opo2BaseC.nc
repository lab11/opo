#include "Opo2Base.h"

configuration Opo2BaseC {}

implementation {
  components MainC, LedsC, SerialStartC, PrintfC, Opo2BaseP;
  Opo2BaseP.Boot -> MainC.Boot;
  Opo2BaseP.Leds -> LedsC;

  components ActiveMessageC;
  components new AMReceiverC(OPO_FLASH_STORE_BASE) as FlashStoreBaseReceive;
  components new AMReceiverC(OPO_FLASH_STORE_BASE_CHECK) as FlashStoreBaseCheck;
  components new AMSenderC(OPO_FLASH_STORE_BASE_CHECK_RECEIVE) as FlashStoreBaseCheckAck;

  Opo2BaseP.FlashStoreBaseReceive -> FlashStoreBaseReceive.Receive;
  Opo2BaseP.FlashStoreBaseCheckReceive -> FlashStoreBaseCheck.Receive;
  Opo2BaseP.FlashStoreBaseCheckAck -> FlashStoreBaseCheckAck.AMSend;

  Opo2BaseP.RfControl -> ActiveMessageC.SplitControl;
  Opo2BaseP.AMPacket -> ActiveMessageC.AMPacket;

  components new LinuxUdpSocketC();
  Opo2BaseP.LinuxUdpSocket -> LinuxUdpSocketC.LinuxUdpSocket;

  components new TimerMilliC() as FlashStoreCheckTimer;
  Opo2BaseP.FlashStoreCheckTimer -> FlashStoreCheckTimer;

}

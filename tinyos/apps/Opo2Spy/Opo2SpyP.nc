#include "Opo2Spy.h"
#include "printf.h"

module Opo2SpyP {
  uses {
    interface Receive as FlashStoreNodeReceive;
    interface SplitControl as RfControl;
    interface AMPacket;
    interface Boot;
    interface Leds;
  }
}

implementation {

  event void Boot.booted() {
    call RfControl.start();
    //printf("Booted\n");
    //printfflush();
  }

  event message_t* FlashStoreNodeReceive.receive(message_t *msg, void *payload, uint8_t len) {
    int i = 0;
    oflash_msg_t *data = (oflash_msg_t *) payload;
    printf("FlashStoreNode %u %u %u", call AMPacket.source(msg), data->seq, data->reset_counter);
    for(i=0;i<8;i++) {
      printf(" %u", data->m_full_time[i]);
    }
    printf("\n");
    printf("%u ", data->last_tx_id);
    printf("%u ", data->dt_ul_rf);
    for(i=0;i<8;i++) {
      printf(" %u", data->last_full_time[i]);
    }
    printf("\n");
    return msg;
  }

  event void RfControl.startDone(error_t err) {
    printf("RfStartDone\n");
    printfflush();
  }
  event void RfControl.stopDone(error_t err) {}

}
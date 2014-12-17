#include "Opo2Spy.h"
#include "printf.h"

#define GATD_HOST "gatd.eecs.umich.edu"
#define GATD_PORT 4001
#define GATD_PROFILE_ID "4wbddZCSIj"

module Opo2SpyP {
  uses {
    interface Receive as FlashStoreNodeReceive;
    interface SplitControl as RfControl;
    interface AMPacket;
    interface Boot;
    interface Leds;
    interface LinuxUdpSocket;
  }
}

implementation {

  event void Boot.booted() {
    call RfControl.start();
    call LinuxUdpSocket.init(GATD_HOST, GATD_PORT);
  }

  event message_t* FlashStoreNodeReceive.receive(message_t *msg, void *payload, uint8_t len) {
    int i = 0;
    oflash_gatd_msg_t m_gatd;
    oflash_gatd_msg_t *gatd_data = &m_gatd;
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

    gatd_data->m_id = call AMPacket.source(msg);
    gatd_data->seq = data->seq;
    gatd_data->reset_counter = data->reset_counter;
    for(i=0;i<8;i++) {
      gatd_data->last_full_time[i] = data->last_full_time[i];
      gatd_data->m_full_time[i] = data->m_full_time[i];
    }
    gatd_data->last_tx_id = data->last_tx_id;
    gatd_data->dt_ul_rf = data->dt_ul_rf;

    // Send a copy of this off to GATD
    if(gatd_data->last_tx_id != 0) {
      if (call LinuxUdpSocket.build_packet(GATD_PROFILE_ID, strlen(GATD_PROFILE_ID)) != SUCCESS) {
        fprintf(stderr, "OpoSpyP::receive\tbuild_packet failed\n");
      }
      if (call LinuxUdpSocket.send_data((uint8_t *) gatd_data, sizeof(oflash_gatd_msg_t)) != SUCCESS) {
        fprintf(stderr, "OpoSpyP::receive\tsend_data failed\n");
      }
    }


    return msg;
  }

  event void RfControl.startDone(error_t err) {}
  event void RfControl.stopDone(error_t err) {}

}
#include "Opo2Base.h"
#include "printf.h"

#define GATD_HOST "inductor.eecs.umich.edu"
#define GATD_PORT 4001
#define GATD_PROFILE_ID "4wbddZCSIj"
#define b_id 1

module Opo2BaseP {
  uses {
    interface Receive as FlashStoreBaseReceive;
    interface Receive as FlashStoreBaseCheckReceive;
    interface AMSend as FlashStoreBaseCheckAck;
    interface Timer<TMilli> as FlashStoreCheckTimer;
    interface SplitControl as RfControl;
    interface AMPacket;
    interface Boot;
    interface Leds;
    interface LinuxUdpSocket;
  }
}

implementation {

  message_t check_packet;
  oflash_base_check_msg_t* cp;
  bool FlashStoreLock = FALSE;

  event void Boot.booted() {
    cp = (oflash_base_check_msg_t*) call FlashStoreBaseCheckAck.getPayload(&check_packet, sizeof(oflash_base_check_msg_t));
    call LinuxUdpSocket.init(GATD_HOST, GATD_PORT);
    call RfControl.start();

  }

  event message_t* FlashStoreBaseReceive.receive(message_t *msg, void *payload, uint8_t len) {
    oflash_base_rf_msg_t *data = (oflash_base_rf_msg_t *) payload;
    uint16_t rx_id = call AMPacket.source(msg);
    oflash2_gatd_msg_t gatd_data;
    oflash2_gatd_msg_t *gd = &gatd_data;
    int i;
    call FlashStoreCheckTimer.stop();

    printf("Opo2FlashStore");
    printf(" %u", rx_id);
    printf(" %u", data->tx_id);
    printf(" %u %i", data->ultrasonic_rf_dt, data->rssi);
    printf(" %u %u %u %u", data->m_reset_counter, data->tx_reset_counter, data->tx_seq, data->rx_fails);
    for(i=0;i<8;i++) {
      printf(" %u", data->full_time[i]);
    }
    for(i=0;i<8;i++) {
      printf(" %u", data->tx_full_time[i]);
    }
    printf("\n");

    gd->rx_id = rx_id;
    gd->tx_id = data->tx_id;
    gd->ultrasonic_rf_dt = data->ultrasonic_rf_dt;
    gd->rssi = data->rssi;
    gd->m_reset_counter = data->m_reset_counter;
    gd->tx_reset_counter = data->tx_reset_counter;
    gd->tx_seq = data->tx_seq;
    gd->rx_fails = data->rx_fails;
    gd->version = 2;
    gd->base_id = b_id;
    for(i=0;i<8;i++) {
      gd->full_time[i] = data->full_time[i];
      gd->tx_full_time[i] = data->tx_full_time[i];
    }

    if (call LinuxUdpSocket.build_packet(GATD_PROFILE_ID, strlen(GATD_PROFILE_ID)) != SUCCESS) {
      fprintf(stderr, "Opo2BaseP::receive\tbuild_packet failed\n");
    }
    if (call LinuxUdpSocket.send_data((uint8_t *) gd, sizeof(oflash2_gatd_msg_t)) != SUCCESS) {
      fprintf(stderr, "Opo2BaseP::receive\tsend_data failed\n");
    }


    call FlashStoreCheckTimer.startOneShot(300);
    return msg;
  }


  event message_t* FlashStoreBaseCheckReceive.receive(message_t *msg, void *payload, uint8_t len) {
    bool check_ack = FALSE;
    uint16_t o_id = call AMPacket.source(msg);
    oflash_base_check_msg_t* p = call FlashStoreBaseCheckAck.getPayload(msg, sizeof(oflash_base_check_msg_t));
    printf("Flash Checker %u \n", call AMPacket.source(msg));

    atomic {
      if (FlashStoreLock == FALSE) {
        FlashStoreLock = TRUE;
        check_ack = TRUE;
      }
    }

    if (check_ack == TRUE) {
      cp->m_id = o_id;
      call FlashStoreBaseCheckAck.send(o_id, &check_packet, sizeof(oflash_base_check_msg_t));
      call FlashStoreCheckTimer.startOneShot(300);
    }

    return msg;
  }

  event void FlashStoreCheckTimer.fired() {
    FlashStoreLock = FALSE;
  }

  event void FlashStoreBaseCheckAck.sendDone(message_t *msg, error_t error) {}

  event void RfControl.startDone(error_t err) {}
  event void RfControl.stopDone(error_t err) {}

}
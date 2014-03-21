#include "OpoSpy.h"
#include "printf.h"

#define GATD_HOST "inductor.eecs.umich.edu"
#define GATD_PORT 4001
#define GATD_MAGIC "4wbddZCSIj"

module OpoSpyP {
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
    //printf("Booted\n");
    //printfflush();
  }

  event message_t* FlashStoreNodeReceive.receive(message_t *msg, void *payload, uint8_t len) {
    int i = 0;
    oflash_msg_t *data = (oflash_msg_t *) payload;
    printf("FlashStoreNode %u %u %u", data->tx_id, data->seq, data->buffer_index);
    for(i=0;i<5;i++) {
      printf(" %u", data->full_time[i]);
    }
    printf("\n");
    printf("%u ", data->last_tx_id);
    printf("%u %u %u %u %u", data->t_ultrasonic, data->t_rf, data->t_ultrasonic_wake, data->t_ultrasonic_wake_falling, data->t_ultrasonic_falling);
    printf("\n");

    // Send a copy of this off to GATD
    if (call LinuxUdpSocket.build_packet(GATD_MAGIC, strlen(GATD_MAGIC)) != SUCCESS) {
      fprintf(stderr, "OpoSpyP::receive\tbuild_packet failed\n");
    }
    if (call LinuxUdpSocket.send_data((uint8_t *) data, sizeof(data)) != SUCCESS) {
      fprintf(stderr, "OpoSpyP::receive\tsend_data failed\n");
    }

    return msg;
  }

  event void RfControl.startDone(error_t err) {
    printf("RfStartDone\n");
    printfflush();
  }
  event void RfControl.stopDone(error_t err) {}

}

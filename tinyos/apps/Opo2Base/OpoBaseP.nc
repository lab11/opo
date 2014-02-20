#include "OpoBase.h"
#include "printf.h"

module OpoBaseP {
  uses {
    interface Receive as OpoReceive;
    interface Receive as BlinkReceive;
    interface Receive as VisualReceive;
    interface Receive as RxReceive;
    interface Receive as ADCReceive;
    interface Receive as FlashReceive;
    interface Receive as SHT25Receive;
    interface Receive as RTCReceive;
    interface Receive as IDReceive;
    interface Receive as FlashReaderReceive;
    interface Receive as FlashReaderTestReceive;
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

  event message_t* OpoReceive.receive(message_t *msg, void *payload, uint8_t len) {
    opo_bmsg_t *data = (opo_bmsg_t *) payload;
    int i;
    uint32_t total_time = 0;
    printf("RANGE:%u\n", data->rec.range);
    printf("SEQ:%u\n", data->rec.sequence);
    printf("RX_ID:0x");
    for(i = 0; i < 6; i++) {
      printf("%x", data->m_id[i]);
    }
    printf("\n");
    printf("TX_ID:0x");
    for(i = 0; i < 6; i++) {
      printf("%x", data->rec.o_id[i]);
    }
    printf("\n");
    printf("RSSI:%i\n", data->rec.rssi);
    printf("TX_PWR:%u\n", data->rec.tx_pwr);
    total_time += data->rec.seconds;
    total_time += data->rec.minutes * 60;
    total_time += data->rec.hours * 60 * 60;
    printf("TIME:%u\n", total_time);
    printfflush();
    return msg;
  }

  event message_t* BlinkReceive.receive(message_t *msg, void *payload, uint8_t len) {
    oblink_base_msg_t *data = (oblink_base_msg_t *) payload;
    int i;

    printf("Range: %u\n", data->range);
    printf("M_ID: 0x");
    for(i=0; i < 6; i++) {
      printf("%x", data->m_id[i]);
    }
    printf("\n");
    printf("O_ID: 0x");
    for(i=0; i < 6; i++) {
      printf("%x", data->o_id[i]);
    }
    printf("\n");

    printfflush();
    return msg;
  }

  event message_t* VisualReceive.receive(message_t *msg, void *payload, uint8_t len) {
    ovis_base_msg_t *data = (ovis_base_msg_t *) payload;
    int i;

    printf("22 ");

    printf("0x");
    for(i=5; i >= 0; i--) {
      printf("%x", data->rx_id[i]);
    }
    printf(" ");

    printf("0x");
    for(i=5; i >= 0; i--) {
      printf("%x", data->tx_id[i]);
    }
    printf(" ");

    printf("%u ", data->full_time);

    printf("%u ", data->ultrasonic_rf_dt);
    printf("%u ", data->ultrasonic_wake_dt);
    printf("%u ", data->ultrasonic_dt);
    printf("%u ", data->seq);
    printf("%u ", data->rx_fails);
    printf("%u\n", data->tx_fails);

    printfflush();
    return msg;
  }

  event message_t* RxReceive.receive(message_t *msg, void *payload, uint8_t len) {
    opo_rx_base_msg_t *data = (opo_rx_base_msg_t *) payload;
    int i;
    float rf_time = (float)data->rx_t_rf;
    float ul_time = (float)data->rx_t_ultrasonic;
    float range = (ul_time - rf_time)/32000.0 * 340.29 - .12;


    printf("0x");
    for(i=7; i>=0; i--) {
      printf("%x", data->rx_id[i]);
    }
    printf("\n");
    printf("0x");
    for(i=7; i>=0; i--) {
      printf("%x", data->tx_id[i]);
    }
    printf("\n");
    printf("Range: %f m\n", range);
    printf("%i", data->rx_rssi);
    printf(" %u", data->rx_t_rf);
    printf(" %u", data->rx_t_ultrasonic);
    printf(" %u", data->tx_seq);
    printf(" %u", data->rx_seq);
    printf(" %u", data->rx_fail_count);
    printf("\n");
    printf("--------------------\n");

    printfflush();
    return msg;
  }

  event message_t* ADCReceive.receive(message_t *msg, void *payload, uint8_t len) {
    opo_adc_msg_t *data = (opo_adc_msg_t *) payload;
    int i;

    for(i = 0; i < 6; i++) {
      printf("24 %u %u %u \n", data->rf_time, data->readings[i], data->times[i]);
    }

    printf("----------------------\n");

    return msg;
  }

  event message_t* FlashReceive.receive(message_t *msg, void *payload, uint8_t len) {
    opo_flash_t *data = (opo_flash_t *) payload;
    int i;
    printf("flashrx\n");

    printf("sid: ");
    for(i = 0; i < 8; i++) {
      printf("%u", data->sid[i]);
    }
    printf("\n");

    printf("readBuffer: ");
    for(i=0; i < 8; i++) {
      printf("%u", data->readBuffer[i]);
    }
    printf("\n");

    printf("status: %u\n", data->statusRegister);

    printf("----------------------\n");

    return msg;
  }

  event message_t* SHT25Receive.receive(message_t *msg, void *payload, uint8_t len) {
    opo_sht25_test_msg_t *data = (opo_sht25_test_msg_t *) payload;
    int i;
    printf("SHT25\n");

    printf("Temp: %u\n", data->temp);

    printf("RH: %u\n", data->rh);
    printf("----------------------\n");

    return msg;
  }

  event message_t* RTCReceive.receive(message_t *msg, void *payload, uint8_t len) {
    opo_rtc_msg_t *data = (opo_rtc_msg_t *) payload;
    uint16_t year = 0;
    int i;

    for(i=0; i<8; i++) {
      printf("%u\n", data->full_time[i]);
    }

    printf("----------------------\n");

    return msg;
  }

  event message_t* IDReceive.receive(message_t *msg, void *payload, uint8_t len) {
    id_store_t *m_id = (id_store_t *) payload;
    printf("ID: %u\n", m_id->id);
    printf("SEED: %u\n", m_id->seed);
    printf("---------------------------\n");
    return msg;
  }

  event message_t* FlashReaderReceive.receive(message_t *msg, void *payload, uint8_t len) {
    int i = 0;
    oflash_base_rf_msg_t *data = (oflash_base_rf_msg_t *) payload;
    printf("FlashStoreReader ");
    printf("%u ", data->tx_seq);
    printf("%u ", data->rx_id);
    printf("%u ", data->tx_id);
    printf("%u ", data->ultrasonic_rf_dt);
    printf("%u ", data->rssi);
    printf("%u ", data->m_seq);
    printf("%u", data->rx_fails);
    for(i=0;i<5;i++) {
      printf(" %u", data->full_time[i]);
    }
    printf("\n");
    for(i=0;i<5;i++) {
      printf(" %u", data->m_full_time[i]);
    }
    printf("\n");
    return msg;
  }

  event message_t* FlashReaderTestReceive.receive(message_t *msg, void *payload, uint8_t len) {
    int i = 0;
    oflash_reader_test_msg_t *data = (oflash_reader_test_msg_t *) payload;
    printf("Base Time: %u\n", data->base_time);
    printf("Current Time: %u\n", data->current_time);
    printf("FlashReaderTest--------------------\n");
    return msg;
  }

  event void RfControl.startDone(error_t err) {
    printf("RfStartDone\n");
    printfflush();
  }
  event void RfControl.stopDone(error_t err) {}

}
#include "Opo2FlashWriter.h"
module Opo2FlashWriterP {
	uses {
		interface Boot;
		interface Leds;
		interface AMSend;
		interface SplitControl;
		interface HplSST25VF064 as FlashHpl;
    interface Timer<TMilli> as FlashTimer;
	}
}
implementation {
    int i = 0;
    message_t packet;
    oflash_base_rf_msg_t *p;
    oflash_base_rf_msg_t buff[7];
		oflash_base_rf_msg_t rbuff[7]
    uint16_t rindex = 0;
    uint32_t page_count = 0;
		bool boot = TRUE;

    event void Boot.booted() {
        p = (oflash_base_rf_msg_t *) call AMSend.getPayload(&packet, sizeof(oflash_base_rf_msg_t));
				for(i=0;i<7;i++) {
					buff[i].tx_id = 11;
					buff[i].ultrasonic_rf_dt = 27;
					buff[i].rssi = 69;
					buff[i].reset_counter = 2;
					buff[i].tx_seq = 4;
					buff[i].rx_fails = 9;
				}
        call SplitControl.start();
    }

    event void FlashHpl.turnedOn() {
			if(boot == TRUE) {
				boot = FALSE;
				call FlashHpl.chip_erase();
			} else {
				page_count = 0;
				call FlashHpl.read(page_count*256, &rbuff, sizeof(rbuff));
			}
    }
    event void FlashHpl.turnedOff() {
			call FlashTimer.startOneShot(2000);
		}

    event void FlashHpl.program_sid_done(void *data, uint8_t tx_len) {}
    event void FlashHpl.read_sid_done(void *rxBuffer, uint8_t rx_len) {}

    event void FlashHpl.chip_erase_done() {
			call FlashHpl.wrsr(0);
			call FlashHpl.page_program(page_count*256, &buff, sizeof(buff));
		}

    event void FlashHpl.page_program_done(void *txBuffer, uint32_t tx_len) {
			page_count++;
			if(page_count < 100) {
				call FlashHpl.page_program(page_count*256, &buff, sizeof(buff));
			} else {
				call FlashHpl.turnOff();
			}
		}

    event void FlashHpl.read_done(void *rxBuffer, uint32_t rx_len) {
        page_count++;
        p->tx_id = buff[rindex].tx_id;
        p->ultrasonic_rf_dt = buff[rindex].ultrasonic_rf_dt;
        p->rssi = buff[rindex].rssi;
        for(i=0; i <8; i++) {
            p->full_time[i] = buff[rindex].full_time[i];
            p->tx_full_time[i] = buff[rindex].tx_full_time[i];
        }
        p->reset_counter = buff[rindex].reset_counter;
        p->tx_seq = buff[rindex].tx_seq;
        p->rx_fails = buff[rindex].rx_fails;
        call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
    }


    event void AMSend.sendDone(message_t *msg, error_t err) {
        if(rindex < 7) {
            p->tx_id = buff[rindex].tx_id;
            p->ultrasonic_rf_dt = buff[rindex].ultrasonic_rf_dt;
            p->rssi = buff[rindex].rssi;
            for(i=0; i <8; i++) {
                p->full_time[i] = buff[rindex].full_time[i];
                p->tx_full_time[i] = buff[rindex].tx_full_time[i];
            }
            p->reset_counter = buff[rindex].reset_counter;
            p->tx_seq = buff[rindex].tx_seq;
            p->rx_fails = buff[rindex].rx_fails;
            rindex++;
            call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(oflash_base_rf_msg_t));
        } else if (page_count > 100) {
            call Leds.led1On();
            call Leds.led0On();
        }
        else {
            rindex = 0;
            call FlashHpl.read(page_count*256, buff, sizeof(buff));
        }
    }

    event void SplitControl.startDone(error_t err) {
        call FlashHpl.turnOn();
    }

    event void FlashTimer.fired() {
			call FlashHpl.turnOn();
    }

    event void SplitControl.stopDone(error_t err) {}
}

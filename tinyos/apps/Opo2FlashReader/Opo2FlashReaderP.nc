#include "Opo2FlashReader.h"
module Opo2FlashReaderP {
	uses {
		interface Boot;
		interface Leds;
		interface AMSend;
		interface SplitControl;
		interface HplSST25VF064 as FlashHpl;
	}
}
implementation {
    int i = 0;
    message_t packet;
    opo_rx_base_msg_t *p;
    uint8_t readBuffer[sizeof(opo_rx_base_msg_t)] = {0};
    uint8_t m_id[8] = {0};
    uint8_t addr[3] = {0,0,0};
    uint32_t current_addr = 0;
    uint32_t z_addr=0;
    uint32_t max_addr = 0x7FFFFF;
    uint8_t status_register = 0;
    uint16_t len = sizeof(opo_rx_base_msg_t);
    uint8_t zeros[256] = {0};

    event void Boot.booted() {
        p = (opo_rx_base_msg_t*) call AMSend.getPayload(&packet, sizeof(opo_rx_base_msg_t));
        call SplitControl.start();
    }

    event void FlashHpl.turnedOn() {
        call FlashHpl.read_sid(addr, &m_id[0], 8);
    }

    event void FlashHpl.turnedOff() {

    }

    event void FlashHpl.read_sid_done(uint8_t *rxBuffer, uint16_t rx_len) {
        call FlashHpl.read(addr, &readBuffer[0], len);
    }

    event void FlashHpl.chip_erase_done() {
        addr[0] = z_addr;
        addr[1] = z_addr >> 8;
        addr[2] = z_addr >> 16;
        z_addr += 256;
        call FlashHpl.page_program(addr, &zeros[0], 256);
    }

    event void FlashHpl.page_program_done(uint8_t *txBuffer, uint16_t page_len) {
        addr[0] = z_addr;
        addr[1] = z_addr >> 8;
        addr[2] = z_addr >> 16;
        if(z_addr <= max_addr - 256) {
            z_addr += 256;
            call Leds.led1On();
            call FlashHpl.page_program(addr, &zeros[0], 256);
        }
        else {
            call Leds.led0On();
        }
    }

    event void FlashHpl.read_done(uint8_t *rxBuffer, uint16_t rx_len) {
        opo_rx_base_msg_t *read_info;
        bool go_on = TRUE;
        read_info = (opo_rx_base_msg_t *) rxBuffer;

        for(i=0; i < 8; i++) {
            p->rx_id[i] = read_info->rx_id[i];
            p->tx_id[i] = read_info->tx_id[i];

            if(p->rx_id[i] != m_id[i]) {
                go_on = FALSE;
            }
        }

        p->rx_t_rf = read_info->rx_t_rf;
        p->rx_t_ultrasonic = read_info->rx_t_ultrasonic;
        p->tx_seq = read_info->tx_seq;
        p->rx_seq = read_info->rx_seq;
        p->rx_rssi = read_info->rx_rssi;
        p->rx_fail_count = read_info->rx_fail_count;

        current_addr += sizeof(opo_rx_base_msg_t);
        addr[0] = current_addr;
        addr[1] = current_addr >> 8;
        addr[2] = current_addr >> 16;
        p->rx_id[0] = addr[0];
        p->rx_id[1] = addr[1];
        p->rx_id[2] = addr[2];

        if(go_on == TRUE) {
            call AMSend.send(AM_BROADCAST_ADDR, &packet, sizeof(opo_rx_base_msg_t));
        }
        else {
            call FlashHpl.wrsr(0);
            call FlashHpl.chip_erase();
        }
    }


    event void AMSend.sendDone(message_t *msg, error_t err) {
        if(current_addr <= (max_addr - len) ) {
            call FlashHpl.read(addr, &readBuffer[0], len);
        }
        else {
            call FlashHpl.wrsr(0);
            call FlashHpl.chip_erase();
        }
    }

    event void SplitControl.startDone(error_t err) {
        call FlashHpl.turnOn();
    }

    event void SplitControl.stopDone(error_t err) {}
}
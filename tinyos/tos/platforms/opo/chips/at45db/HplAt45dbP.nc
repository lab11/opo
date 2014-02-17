#include "HplAt45db.h"
module HplAt45dbP {
	provides {
		interface HplAt45db;
	}
	uses {
		interface Leds;
		interface Resource as SpiResource;
		interface SpiByte;
		interface SpiPacket;
		interface GeneralIO as FlashCS;
		interface GeneralIO as FlashReset;
		interface Timer<TMilli> as WaitTimer;
		interface BusyWait<TMicro, uint16_t>;
	}
}
implementation {
	enum {
		AT45DB_IDLE,
		AT45DB_PAGE_PROGRAM_ERASE_1,
		AT45DB_PAGE_PROGRAM_ERASE_2,
		AT45DB_DEEP_POWER_DOWN,
		AT45DB_RESUME
	} m_state;

	m_state = AT45DB_IDLE;
	uint8_t addr[3];

	event void SpiPacket.sendDone(uint8_t *txBuf,
								  uint8_t *rxBuf,
								  uint16_t len,
								  error_t error) {
		if(HplAt45dbState )
	}


	command void HplAt45db.turnOn() {
		call SpiByte.write(RESUME_FROM_PD);
		call BusyWait.wait(STANDBY_US+1);
		signal HplAt45db.turnedOn();
	}
	command void HplAt45db.turnOff() {
		call SpiByte.write(DEEP_POWER_DOWN);
		call BusyWait.wait(DEEP_POWER_DOWN_US+1);
		signal HplAt45db.turnedOff();

	}

	command void HplAt45db.write_buffer_1(uint8_t *b, uint16_t len) {

	}
	command void HplAt45db.write_buffer_2(uint8_t *b, uint16_t len) {

	}

	command void HplAt45db.flush_buffer_1(uint16_t page_addr) {
		m_state = AT45DB_PAGE_PROGRAM_ERASE_1;
		shiftPageAddr(page_addr);
		call SpiByte.write(BUFFER_TO_MEM_ERASE_1);
		call SpiByte.write(addr[0]);
		call SpiByte.write(addr[1]);
		call SpiByte.write(addr[2]);
		call WaitTimer.startOneShot(PAGE_ERASE_AND_PROGRAM_MS);
	}
	command void HplAt45db.flush_buffer_2(uint16_t page_addr) {
		shiftPageAddr(page_addr);
		m_state = AT45DB_PAGE_PROGRAM_ERASE_2;
		call SpiByte.write(BUFFER_TO_MEM_ERASE_2);
		call SpiByte.write(addr[0]);
		call SpiByte.write(addr[1]);
		call SpiByte.write(addr[2]);
		call WaitTimer.startOneShot(PAGE_ERASE_AND_PROGRAM_MS);
	}

	event void WaitTimer.fired() {
		if(m_state == AT45DB_PAGE_PROGRAM_ERASE_1) {
			signal HplAt45db.flush_buffer_1_done();
		}
		if(m_state == AT45DB_PAGE_PROGRAM_ERASE_2) {
			signal HplAt45db.flush_buffer_2_done();
		}
	}

	void shiftPageAddr(uint16_t page_addr) {
		addr[0] = page_addr >> 10;
		addr[0] &= 0x3f;
		addr[1] = page_addr << 12;
		addr[1] &= 0xf0;
		addr[2] = 0;
	}
}
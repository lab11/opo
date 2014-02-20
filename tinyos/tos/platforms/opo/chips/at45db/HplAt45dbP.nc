#include "HplAt45db.h"
module HplAt45dbP {
	provides {
		interface HplAt45db;
	}
	uses {
		interface Leds;
		interface Resource as SpiResource;
		interface SpiByte;
		interface GeneralIO as FlashCS;
		interface GeneralIO as FlashReset;
		interface Timer<TMilli> as WaitTimer;
		interface BusyWait<TMicro, uint16_t>;
	}
}
implementation {
	typedef enum {
		AT45DB_IDLE,
		AT45DB_PAGE_PROGRAM_ERASE_1,
		AT45DB_PAGE_PROGRAM_ERASE_2,
		AT45DB_DEEP_POWER_DOWN,
		AT45DB_RESUME
	} at45db_state_t;

	at45db_state_t at45db_state = AT45DB_IDLE;

	uint8_t addr[3];
	uint16_t i = 0;

	void shiftPageAddr(uint16_t page_addr);


	command void HplAt45db.turnOn() {
		at45db_state = AT45DB_RESUME;
		call SpiResource.request();
	}
	command void HplAt45db.turnOff() {
		call FlashCS.clr();
		call SpiByte.write(DEEP_POWER_DOWN);
		call FlashCS.set();
		at45db_state = AT45DB_DEEP_POWER_DOWN;
		call WaitTimer.startOneShot(1);
		//call BusyWait.wait(DEEP_POWER_DOWN_US+1);
		//call SpiResource.release();
		//signal HplAt45db.turnedOff();
	}

	event void SpiResource.granted() {
		if(at45db_state == AT45DB_RESUME) {
			 call FlashCS.clr();
			call SpiByte.write(RESUME_FROM_PD);
			call FlashCS.set();
			call WaitTimer.startOneShot(1);
		}
		//call BusyWait.wait(STANDBY_US+1);
		//call Leds.led1Toggle();
		//signal HplAt45db.turnedOn();
	}

	command void HplAt45db.read(uint16_t page_addr, void *rxBuffer, uint16_t rx_len) {
		shiftPageAddr(page_addr);
		call FlashCS.clr();
		call SpiByte.write(CONTINUOUS_ARRAY_READ_LOW_FREQ);
		call SpiByte.write(addr[0]);
		call SpiByte.write(addr[1]);
		call SpiByte.write(addr[2]);
		for(i=0;i<rx_len;i++) {
			((uint8_t *)rxBuffer)[i] = call SpiByte.write(0);
		}
		call FlashCS.set();
		signal HplAt45db.read_done(rxBuffer, rx_len);
	}

	command void HplAt45db.write_buffer_1(void *b, uint16_t len) {
		addr[0] = 0;
		addr[1] = 0;
		addr[2] = 0;
		call FlashCS.clr();
		call SpiByte.write(BUFFER_WRITE_1);
		call SpiByte.write(addr[0]);
		call SpiByte.write(addr[1]);
		call SpiByte.write(addr[2]);
		for(i=0;i<len;i++) {
			call SpiByte.write(((uint8_t *)b)[i]);
		}
		call FlashCS.set();
		signal HplAt45db.write_buffer_1_done();
	}
	command void HplAt45db.write_buffer_2(void *b, uint16_t len) {
		addr[0] = 0;
		addr[1] = 0;
		addr[2] = 0;
		call FlashCS.clr();
		call SpiByte.write(BUFFER_WRITE_2);
		call SpiByte.write(addr[0]);
		call SpiByte.write(addr[1]);
		call SpiByte.write(addr[2]);
		for(i=0;i<len;i++) {
			call SpiByte.write(((uint8_t *)b)[i]);
		}
		call FlashCS.set();
		signal HplAt45db.write_buffer_2_done();
	}

	command void HplAt45db.flush_buffer_1(uint16_t page_addr) {
		shiftPageAddr(page_addr);
		at45db_state = AT45DB_PAGE_PROGRAM_ERASE_1;
		call FlashCS.clr();
		call SpiByte.write(BUFFER_TO_MEM_ERASE_1);
		call SpiByte.write(addr[0]);
		call SpiByte.write(addr[1]);
		call SpiByte.write(addr[2]);
		call FlashCS.set();
		call WaitTimer.startOneShot(PAGE_ERASE_AND_PROGRAM_MS+10);
	}
	command void HplAt45db.flush_buffer_2(uint16_t page_addr) {
		shiftPageAddr(page_addr);
		at45db_state = AT45DB_PAGE_PROGRAM_ERASE_2;
		call FlashCS.clr();
		call SpiByte.write(BUFFER_TO_MEM_ERASE_2);
		call SpiByte.write(addr[0]);
		call SpiByte.write(addr[1]);
		call SpiByte.write(addr[2]);
		call FlashCS.set();
		call WaitTimer.startOneShot(PAGE_ERASE_AND_PROGRAM_MS);
	}

	event void WaitTimer.fired() {
		if(at45db_state == AT45DB_PAGE_PROGRAM_ERASE_1) {
			at45db_state = AT45DB_IDLE;
			signal HplAt45db.flush_buffer_1_done();
		}
		else if(at45db_state == AT45DB_PAGE_PROGRAM_ERASE_2) {
			at45db_state = AT45DB_IDLE;
			signal HplAt45db.flush_buffer_2_done();
		}
		else if(at45db_state == AT45DB_RESUME) {
			at45db_state = AT45DB_IDLE;
			signal HplAt45db.turnedOn();
		}
		else if(at45db_state == AT45DB_DEEP_POWER_DOWN) {
			at45db_state = AT45DB_IDLE;
			call SpiResource.release();
			signal HplAt45db.turnedOff();
		}
	}

	command uint8_t* HplAt45db.get_addr() {
		return &addr[0];
	}

	void shiftPageAddr(uint16_t page_addr) {
		addr[0] = page_addr >> 6;
		addr[0] &= 0x3f;
		addr[1] = page_addr << 2;
		addr[1] &= 0xfc;
		addr[2] = 0;
	}
}
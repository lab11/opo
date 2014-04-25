#include "HplSST25VF064.h"

/*
Because this implementation uses the SpiByte interface,
the cmdBuffer and RxBuffer must both be greater than
len(tx_msg) + len(rx_msg)
*/


module HplSST25VF064P {
	provides {
		interface HplSST25VF064;
	}
	uses {
		interface Resource as SpiResource;
		interface SpiByte;
		interface GeneralIO as FlashPowerGate;
		interface GeneralIO as FlashCS;
		interface GeneralIO as ResetHoldPin;
		interface GeneralIO as WriteProtectPin;
		interface BusyWait<TMicro, uint16_t>;
		interface Leds;
		interface Timer<TMilli> as WaitTimer;
	}
}
implementation {

	enum {PAGE_PROGRAM_DONE, CHIP_ERASE_DONE, SMALL_BLOCK_ERASE_DONE, PROGRAM_SID_DONE, IDLE} status = IDLE;
	uint16_t len;
	uint8_t *buffer;
	uint8_t m_addr[3];
	uint8_t i;

	inline void runSpiByteRx(uint8_t *cmdBuffer, void *rxBuffer, uint32_t rx_len) {
		call FlashCS.clr();
		for(i = 0; i < 4; i++) {
			call SpiByte.write(cmdBuffer[i]);
		}
		for(i = 0; i < rx_len; i++) {
			((uint8_t *)rxBuffer)[i] = call SpiByte.write(0);
		}
		call FlashCS.set();
	}

	inline void runSpiByteTx(uint8_t *cmdBuffer, void *txBuffer, uint32_t tx_len) {
		call FlashCS.clr();
		for(i = 0; i < 4; i++) {
			call SpiByte.write(cmdBuffer[i]);
		}
		for(i = 0; i < tx_len; i++) {
			call SpiByte.write(((uint8_t *)txBuffer)[i]);
		}
		call FlashCS.set();
	}

	inline void runSingleCommand(uint8_t cmd) {
		call FlashCS.clr();
		call SpiByte.write(cmd);
		call FlashCS.set();
	}

	inline void shiftPageAddr(uint32_t user_addr) {
		m_addr[0] = user_addr >> 16;
		m_addr[1] = user_addr >> 8;
		m_addr[2] = user_addr;
	}

	event void WaitTimer.fired() {
		if(status == PAGE_PROGRAM_DONE) {
			signal HplSST25VF064.page_program_done(buffer, len);
		}
		else if(status == CHIP_ERASE_DONE) {
			signal HplSST25VF064.chip_erase_done();
		}
		else if(status == PROGRAM_SID_DONE) {
			signal HplSST25VF064.program_sid_done(buffer, len);
		}
	}

	command void HplSST25VF064.turnOn() {
		call ResetHoldPin.makeOutput();
		call FlashPowerGate.makeOutput();
		call WriteProtectPin.makeOutput();
		call FlashCS.makeOutput();
		call ResetHoldPin.clr();
		call FlashPowerGate.clr();
		call BusyWait.wait(150);
		call ResetHoldPin.set();
		call WriteProtectPin.set();
		call FlashCS.clr();
		call SpiResource.request();
	}

	command void HplSST25VF064.turnOff() {
		call SpiResource.release();
		call FlashPowerGate.set();
		call ResetHoldPin.clr();
		call FlashCS.clr();
		signal HplSST25VF064.turnedOff();
	}

	event void SpiResource.granted() {
		call FlashCS.set();
		signal HplSST25VF064.turnedOn();
	}

	command void HplSST25VF064.read(uint32_t addr, void *rxBuffer, uint32_t rx_len) {
		uint8_t cmdBuffer[4];
		shiftPageAddr(addr);
		cmdBuffer[0] = READ;
		cmdBuffer[1] = m_addr[0];
		cmdBuffer[2] = m_addr[1];
		cmdBuffer[3] = m_addr[2];

		runSpiByteRx(&cmdBuffer[0], rxBuffer, rx_len);

		signal HplSST25VF064.read_done(rxBuffer, rx_len);
	}

	command void HplSST25VF064.read_sid(uint8_t addr, void *rxBuffer, uint8_t rx_len) {
		uint8_t cmdBuffer[3];
		cmdBuffer[0] = READ_SID;
		cmdBuffer[1] = addr;
		cmdBuffer[2] = 0;

		call FlashCS.clr();
		call SpiByte.write(cmdBuffer[0]);
		call SpiByte.write(cmdBuffer[1]);
		call SpiByte.write(cmdBuffer[2]);
		for(i = 0; i < rx_len; i++) {
			((uint8_t *)rxBuffer)[i] = call SpiByte.write(0);
		}
		call FlashCS.set();

		signal HplSST25VF064.read_sid_done(rxBuffer, rx_len);
	}

	command void HplSST25VF064.lock_sid() {

	}

	command void HplSST25VF064.program_sid(uint8_t addr, void *txBuffer, uint8_t tx_len) {
		uint8_t cmdBuffer[2];
		uint8_t check_status = 0;
		cmdBuffer[0] = PROGRAM_SID;
		cmdBuffer[1] = addr;

		call HplSST25VF064.write_enable();

		call FlashCS.clr();
		call SpiByte.write(cmdBuffer[0]);
		call SpiByte.write(cmdBuffer[1]);
		for(i = 0; i < tx_len; i++) {
			call SpiByte.write(((uint8_t *)txBuffer)[i]);
		}
		call FlashCS.set();

		status = PROGRAM_SID_DONE;
		len = tx_len;
		buffer = txBuffer;
		call WaitTimer.startOneShot(T_PROGRAM_SID);
	}

	command uint8_t HplSST25VF064.read_status_register() {
		uint8_t status_buffer = 0;
		call FlashCS.clr();
		call SpiByte.write(RDSR);
		status_buffer = call SpiByte.write(0);
		call FlashCS.set();

		return status_buffer;
	}

	command void HplSST25VF064.write_enable() {
		runSingleCommand(WREN);
	}

	command void HplSST25VF064.write_disable() {
		runSingleCommand(WRDI);
	}

	/*
		The write_enable command must be called prior to page program.
	*/
	command void HplSST25VF064.page_program(uint32_t addr,
											void *txBuffer,
											uint32_t tx_len) {
		uint8_t cmdBuffer[4];
		shiftPageAddr(addr);
		cmdBuffer[0] = PAGE_PROGRAM;
		cmdBuffer[1] = m_addr[0];
		cmdBuffer[2] = m_addr[1];
		cmdBuffer[3] = m_addr[2];

		call HplSST25VF064.write_enable();
		runSpiByteTx(&cmdBuffer[0], txBuffer, tx_len);

		status = PAGE_PROGRAM_DONE;
		len = tx_len;
		buffer = txBuffer;

		call WaitTimer.startOneShot(T_PAGE_PROGRAM);
	}

	command void HplSST25VF064.chip_erase() {
		call HplSST25VF064.write_enable();

		runSingleCommand(CHIP_ERASE);
		status = CHIP_ERASE_DONE;
		call WaitTimer.startOneShot(T_CHIP_ERASE);
	}

	command void HplSST25VF064.ewsr() {
		runSingleCommand(EWSR);
	}

	command void HplSST25VF064.wrsr(uint8_t status_data) {
		call HplSST25VF064.ewsr();
		call FlashCS.clr();
		call SpiByte.write(WRSR);
		call SpiByte.write(status_data);
		call FlashCS.set();
	}

	command void HplSST25VF064.ehld() {} // enable hold pin. turns reset pin into hold pin
	command void HplSST25VF064.rdid() {} // reads the manufacturer and device id
}
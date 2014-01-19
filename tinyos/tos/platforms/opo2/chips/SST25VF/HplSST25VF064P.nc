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

	int i; // for loop variable
	enum {PAGE_PROGRAM_DONE, CHIP_ERASE_DONE, SMALL_BLOCK_ERASE_DONE, IDLE} status = IDLE;
	uint16_t len;
	uint8_t *buffer;

	inline void runSpiByteRx(uint8_t *cmdBuffer, uint8_t *rxBuffer, uint16_t rx_len) {
		call FlashCS.clr();
		for(i = 0; i < 4; i++) {
			call SpiByte.write(cmdBuffer[i]);
		}
		for(i = 0; i < rx_len; i++) {
			rxBuffer[i] = call SpiByte.write(0);
		}
		call FlashCS.set();
	}

	inline void runSpiByteTx(uint8_t *cmdBuffer, uint8_t *txBuffer, uint16_t tx_len) {
		call FlashCS.clr();
		for(i = 0; i < 4; i++) {
			call SpiByte.write(cmdBuffer[i]);
		}
		for(i = 0; i < tx_len; i++) {
			call SpiByte.write(txBuffer[i]);
		}
		call FlashCS.set();
	}

	inline void runSingleCommand(uint8_t cmd) {
		call FlashCS.clr();
		call SpiByte.write(cmd);
		call FlashCS.set();
	}

	event void WaitTimer.fired() {
		if(status == PAGE_PROGRAM_DONE) {
			signal HplSST25VF064.page_program_done(buffer, len);
		}
		else if(status == CHIP_ERASE_DONE) {
			signal HplSST25VF064.chip_erase_done();
		}
	}

	command void HplSST25VF064.turnOn() {
		call FlashPowerGate.clr();
		call ResetHoldPin.set();
		call WriteProtectPin.makeOutput();
		call WriteProtectPin.set();
		call BusyWait.wait(150);
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
		signal HplSST25VF064.turnedOn();
	}

	command void HplSST25VF064.read(uint8_t addr[3], uint8_t *rxBuffer, uint16_t rx_len) {
		uint8_t cmdBuffer[4];
		cmdBuffer[0] = READ;
		cmdBuffer[1] = addr[0];
		cmdBuffer[2] = addr[1];
		cmdBuffer[3] = addr[2];

		runSpiByteRx(&cmdBuffer[0], rxBuffer, rx_len);

		signal HplSST25VF064.read_done(rxBuffer, rx_len);
	}

	command void HplSST25VF064.high_speed_read(uint8_t addr[3], uint8_t *cmdBuffer, uint8_t *rxBuffer, uint16_t rx_len) {}
	command void HplSST25VF064.fast_read_dual_output(uint8_t addr[3], uint8_t *cmdBuffer, uint8_t *rxBuffer, uint16_t rx_len) {}

	command void HplSST25VF064.read_sid(uint8_t addr[3], uint8_t *rxBuffer, uint16_t rx_len) {
		uint8_t cmdBuffer[4];
		cmdBuffer[0] = READ_SID;
		cmdBuffer[1] = addr[0];
		cmdBuffer[2] = addr[1];
		cmdBuffer[3] = addr[2];
		runSpiByteRx(&cmdBuffer[0], rxBuffer, rx_len);
		signal HplSST25VF064.read_sid_done(rxBuffer, rx_len);
	}

	command void HplSST25VF064.lock_sid() {

	}

	command void HplSST25VF064.program_sid(uint8_t addr[3], uint8_t *data, uint16_t tx_len) {

	}

	command void HplSST25VF064.read_status_register() {

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
	command void HplSST25VF064.page_program(uint8_t addr[3],
											uint8_t *txBuffer,
											uint16_t tx_len) {
		uint8_t cmdBuffer[4];
		cmdBuffer[0] = PAGE_PROGRAM;
		cmdBuffer[1] = addr[0];
		cmdBuffer[2] = addr[1];
		cmdBuffer[3] = addr[2];

		runSpiByteTx(&cmdBuffer[0], txBuffer, tx_len);

		status = PAGE_PROGRAM_DONE;
		len = tx_len;
		buffer = txBuffer;

		call WaitTimer.startOneShot(T_PAGE_PROGRAM);
	}

	command void HplSST25VF064.dual_input_page_program(uint8_t addr[3], uint8_t *cmdBuffer, uint16_t rx_len) {}

	command void HplSST25VF064.sector_erase(uint8_t addr[3]) {}
	command void HplSST25VF064.small_block_erase(uint8_t addr[3]) {}
	command void HplSST25VF064.large_block_erase(uint8_t addr[3]) {}

	command void HplSST25VF064.chip_erase() {
		runSingleCommand(CHIP_ERASE);
		status = CHIP_ERASE_DONE;
		call WaitTimer.startOneShot(T_CHIP_ERASE);
	}

	command void HplSST25VF064.ewsr() {} // Enable write status register
	command void HplSST25VF064.wrsr(uint8_t *data) {} // write status regsiter

	command void HplSST25VF064.ehld() {} // enable hold pin. turns reset pin into hold pin
	command void HplSST25VF064.rdid() {} // reads the manufacturer and device id


}
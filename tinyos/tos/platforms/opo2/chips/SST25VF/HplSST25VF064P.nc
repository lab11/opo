#include "HplSST25VF064.h"

/*
Because this implementation uses the SpiByte interface,
the TxBuffer and RxBuffer must both be greater than
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
		interface BusyWait<TMicro, uint16_t>;
		interface Leds;
	}
}
implementation {

	int i; // for loop variable
	uint8_t cmd_buffer[3];

	inline void runSpiByte(uint8_t *txBuffer, uint16_t tx_len uint8_t *rxBuffer, uint16_t rx_len) {
		call FlashCS.clr();
		for(i = 0; i < tx_len; i++) {
			call SpiByte.write(txBuffer[i])
		}
		for(i = 0; i < rx_len; i++) {
			rxBuffer[i] = call SpiByte.write(txBuffer[i]);
		}
		call FlashCS.set();
	}

	inline void leftShiftRxBuffer(uint8_t *rxBuffer, uint16_t len, uint16_t shift) {
		for(i=0; i < (len - shift); i++) {
			rxBuffer[i] = rxBuffer[i + shift];
		}
	}

	inline void runSingleCommand(uint8_t cmd) {
		cmd_buffer[0] = cmd;
		call FlashCS.clr();
		call SpiByte.write(cmd_buffer[0]);
		call FlashCS.set();
	}

	command void HplSST25VF064.turnOn() {
		call FlashPowerGate.clr();
		call ResetHoldPin.set();
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

	command void HplSST25VF064.read(uint8_t addr[3], uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t len) {
		txBuffer[0] = READ;
		txBuffer[1] = addr[0];
		txBuffer[2] = addr[1];
		txBuffer[3] = addr[2];

		runSpiByte(txBuffer, rxBuffer, len);
		leftShiftRxBuffer(rxBuffer, len, 4);
	}

	command void HplSST25VF064.high_speed_read(uint8_t addr[3], uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t len) {}
	command void HplSST25VF064.fast_read_dual_output(uint8_t addr[3], uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t len) {}

	command void HplSST25VF064.read_sid(uint8_t addr[3], uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t len) {
		txBuffer[0] = READ_SID;
		txBuffer[1] = addr[0];
		txBuffer[2] = addr[1];
		txBuffer[3] = addr[2];
		runSpiByte(txBuffer, rxBuffer, len);
		leftShiftRxBuffer(rxBuffer, len, 4);
	}

	command void HplSST25VF064.lock_sid() {

	}

	command void HplSST25VF064.program_sid(uint8_t addr[3], uint8_t *data, uint16_t len) {

	}

	command void HplSST25VF064.read_status_register() {}

	command void HplSST25VF064.write_enable() {
		runSingleCommand(WREN);
	}

	command void HplSST25VF064.write_disable() {
		runSingleCommand(WRDI);
	}

	command void HplSST25VF064.page_program(uint8_t addr[3], uint8_t *txBuffer, uint16_t len) {


	}

	command void HplSST25VF064.dual_input_page_program(uint8_t addr[3], uint8_t *txBuffer, uint16_t len) {}

	command void HplSST25VF064.sector_erase(uint8_t addr[3]) {}
	command void HplSST25VF064.small_block_erase(uint8_t addr[3]) {}
	command void HplSST25VF064.large_block_erase(uint8_t addr[3]) {}

	command void HplSST25VF064.chip_erase() {
		runSingleCommand(CHIP_ERASE);
	}

	command void HplSST25VF064.ewsr() {} // Enable write status register
	command void HplSST25VF064.wrsr(uint8_t *data) {} // write status regsiter

	command void HplSST25VF064.ehld() {} // enable hold pin. turns reset pin into hold pin
	command void HplSST25VF064.rdid() {} // reads the manufacturer and device id


}
#include "contiki.h"
#include "sst25vf.h"
#include "spi-arch.h"
#include "spi.h"
#include "ssi.h"
#include "dev/ssi.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/leds.h"
#include "sys/clock.h"
#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#include "cc2538-rf.h"
#include "cc2538-rf-debug.h"
//#include "cloudcomm.h"

static enum {CHIP_ERASE_DONE,
	  		 SMALL_BLOCK_ERASE_DONE,
	  		 LARGE_BLOCK_ERASE_DONE,
	  		 SECTOR_ERASE_DONE,
	  		 PROGRAM_SID_DONE,
	  		 IDLE} status = IDLE;

static uint32_t i = 0;
static uint8_t  m_addr[3];

static bool on = false;

void sst25vf_default_callback() {}

static inline void set_flash_cs() {
	SPI_CS_SET(SST25VF_CS_PORT_NUM, SST25VF_CS_PIN_NUM);
}

static inline void clr_flash_cs() {
	SPI_CS_CLR(SST25VF_CS_PORT_NUM, SST25VF_CS_PIN_NUM);
}

static inline void clr_flash_power_pin() {
	GPIO_CLR_PIN(SST25VF_FLASH_POWER_PORT_BASE, SST25VF_FLASH_POWER_PIN_MASK);
}

static inline void set_flash_power_pin() {
	GPIO_SET_PIN(SST25VF_FLASH_POWER_PORT_BASE, SST25VF_FLASH_POWER_PIN_MASK);
}

static inline void clr_flash_reset_pin() {
	GPIO_CLR_PIN(SST25VF_FLASH_RESET_PORT_BASE, SST25VF_FLASH_RESET_PIN_MASK);
}

static inline void set_flash_reset_pin() {
	GPIO_SET_PIN(SST25VF_FLASH_RESET_PORT_BASE, SST25VF_FLASH_RESET_PIN_MASK);
}

static inline void enable_writes() {
	sst25vf_ewsr();
	sst25vf_write_status_register(0);
	sst25vf_write_enable();
}


static inline void runSpiByteRx(uint8_t cmdBuffer[4], uint8_t *rxBuffer, uint32_t rx_len) {
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	//SPI_FLUSH();
	if(!spi_flush_buffer()) {send_rf_debug_msg("runSpiByteRx flush 0 fail");}
	clr_flash_cs();
	uint32_t j;

	for(j = 0; j < 4; j++) {
		if(!spi_write_byte(cmdBuffer[j])) {send_rf_debug_msg("runSpiByteRx tx fail");}
		if(!spi_flush_buffer()) {send_rf_debug_msg("runSpiByteRx flush 1 fail");}
		//SPI_WRITE(cmdBuffer[j]);
		//SPI_FLUSH();
	}
	for(j = 0; j < rx_len; j++) {
		if(!spi_read_byte(&(rxBuffer[j]))) {send_rf_debug_msg("runSpiByteRx rx fail");}
		if(!spi_flush_buffer()) {send_rf_debug_msg("runSpiByteRx flush 2 fail");}
		//SPI_READ(((uint8_t *)rxBuffer)[j]);
		//SPI_FLUSH();
	}
	set_flash_cs();
}

static void runSpiByteTx(uint8_t cmdBuffer[4], uint8_t *txBuffer, uint32_t tx_len) {
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	if(!spi_flush_buffer()) {send_rf_debug_msg("runSpiByteTx flush 0 fail");}
	//SPI_FLUSH();
	clr_flash_cs();
	uint32_t j;
	for(j = 0; j < 4; j++) {
		if(!spi_write_byte(cmdBuffer[j])) {send_rf_debug_msg("runSpiByteTx write 1 fail");}
		if(!spi_flush_buffer()) {send_rf_debug_msg("runSpiByteTx flush 1 fail");}
		//SPI_WRITE(cmdBuffer[j]);
		//SPI_FLUSH();
	}
	if (txBuffer != NULL) {
		for(j = 0; j < tx_len; j++) {
			if(!spi_write_byte(txBuffer[j])) {send_rf_debug_msg("runSpiByteTx write 2 fail");}
			if(!spi_flush_buffer()) {send_rf_debug_msg("runSpiByteTx flush 2 fail");}
			//SPI_WRITE(((uint8_t *)txBuffer)[j]);
			//SPI_FLUSH();
		}
	}
	set_flash_cs();
}

static inline void shiftPageAddr(uint32_t user_addr) {
	m_addr[0] = user_addr >> 16;
	m_addr[1] = user_addr >> 8;
	m_addr[2] = user_addr;
}

static inline void runSingleCommand(uint8_t cmd) {
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	//SPI_FLUSH();
	spi_flush_buffer();
	clr_flash_cs();
	spi_write_byte(cmd);
	spi_flush_buffer();
	//SPI_WRITE(cmd);
	//SPI_FLUSH();
	set_flash_cs();
}

static bool check_sst25vf_busy() {
	uint32_t counter = 0;
	bool is_success = true;
	while(sst25vf_read_status_register() & STATUS_BUSY) {
		clock_delay_usec(500);
		if(++counter == 121) {
			is_success = false;
			break;
		}
	}
	return is_success;
}

void sst25vf_turn_on() {
	clr_flash_reset_pin();
	set_flash_cs();
	clr_flash_power_pin();
	clock_delay_usec(150);
	set_flash_reset_pin();
	on = true;
}

void sst25vf_turn_off() {
	set_flash_power_pin();
	clr_flash_reset_pin();
	clr_flash_cs();
	on = false;
}

uint8_t sst25vf_read_page(uint32_t addr, uint8_t *rxBuffer, uint32_t rx_len) {
	bool auton = false;
	INTERRUPTS_DISABLE();
	if(!on) {
		auton = true;
		sst25vf_turn_on();
	}
	if(!check_sst25vf_busy()) {
		send_rf_debug_msg("sst25vf_read_page fail 1");
		return 0;
	}

	shiftPageAddr(addr);
	uint8_t cmdBuffer[4];
	cmdBuffer[0] = READ;
	cmdBuffer[1] = m_addr[0];
	cmdBuffer[2] = m_addr[1];
	cmdBuffer[3] = m_addr[2];
	runSpiByteRx(cmdBuffer, rxBuffer, rx_len);
	if(!check_sst25vf_busy()) {
		send_rf_debug_msg("sst25vf_read_page fail 2");
		return 0;
	}

	if(auton) { sst25vf_turn_off(); }
	INTERRUPTS_ENABLE();

	return 1;
}

void sst25vf_read_sid(uint8_t addr, void *rxBuffer, uint8_t rx_len) {
	uint8_t cmdBuffer[3];
	cmdBuffer[0] = READ_SID;
	cmdBuffer[1] = addr;
	cmdBuffer[2] = 0;
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	SPI_FLUSH();
	clr_flash_cs();
	SPI_WRITE(cmdBuffer[0]);
	SPI_WRITE(cmdBuffer[1]);
	SPI_WRITE(cmdBuffer[2]);
	SPI_FLUSH();
	for(i = 0; i < rx_len; i++) {
		SPI_READ(((uint8_t *)rxBuffer)[i]);
	}
	SPI_FLUSH();
	set_flash_cs();
}

void sst25vf_program_sid(uint8_t addr, void *txBuffer, uint8_t tx_len) {
	// DON'T USE THIS. UNTESTED
	uint8_t cmdBuffer[2];
	cmdBuffer[0] = PROGRAM_SID;
	cmdBuffer[1] = addr;

	enable_writes();
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	SPI_FLUSH();
	clr_flash_cs();
	SPI_WRITE(cmdBuffer[0]);
	SPI_WRITE(cmdBuffer[1]);
	for(i = 0; i < tx_len; i++) {
		SPI_WRITE(((uint8_t *)txBuffer)[i]);
	}
	set_flash_cs();

	status = PROGRAM_SID_DONE;
}

uint8_t sst25vf_read_status_register() {
	uint8_t status_buffer = 0;
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	//SPI_FLUSH();
	spi_flush_buffer();
	clr_flash_cs();
	spi_write_byte(RDSR);
	spi_flush_buffer();
	spi_read_byte(&status_buffer);
	spi_flush_buffer();
	//SPI_WRITE(RDSR);
	//SPI_FLUSH();
	//SPI_READ(status_buffer);
	//SPI_FLUSH();
	set_flash_cs();
	return status_buffer;

}

void sst25vf_write_enable() {
	runSingleCommand(WREN);
}

void sst25vf_write_disable() {
	runSingleCommand(WRDI);
}

uint8_t sst25vf_program(uint32_t addr, uint8_t *txBuffer, uint32_t tx_len) {
	uint8_t cmdBuffer[4];
	bool auton = false;
	bool is_success = true;
	INTERRUPTS_DISABLE();
	if(!on) {
		auton = true;
		sst25vf_turn_on();
	}
	if(check_sst25vf_busy()) {
		shiftPageAddr(addr);
		cmdBuffer[0] = PAGE_PROGRAM;
		cmdBuffer[1] = m_addr[0];
		cmdBuffer[2] = m_addr[1];
		cmdBuffer[3] = m_addr[2];

		enable_writes();
		uint32_t counter = 0;
		while(sst25vf_read_status_register() != STATUS_WEL) {
			clock_delay_usec(500);
			if(++counter == 121) {
				is_success = false;
				break;
			}
		}
		if(is_success) {
			runSpiByteTx(cmdBuffer, txBuffer, tx_len);
			clock_delay_usec(3000);
			if(!check_sst25vf_busy()) {is_success = false;}
		}

	}
	else { is_success = false; }
	spi_flush_buffer();
	//SPI_FLUSH();

	if(auton) { sst25vf_turn_off(); }
	if(!is_success) { send_rf_debug_msg("sst25vf_program fail"); }
	INTERRUPTS_ENABLE();

	return 1;
}

uint8_t sst25vf_chip_erase() {
	bool auton = false;

	if(!on) {
		auton = true;
		sst25vf_turn_on();
	}

	if(!check_sst25vf_busy()) {
		send_rf_debug_msg("sst25vf_chip_erase fail 1");
		if(auton == false) {sst25vf_turn_off();}
		return 0;
	}

	enable_writes();
	runSingleCommand(CHIP_ERASE);
	clock_delay_usec(51000);
	if(!check_sst25vf_busy()) {
		send_rf_debug_msg("sst25vf_chip_erase fail 2");
		if(auton == false) {sst25vf_turn_off();}
		return 0;
	}

	if(auton) { sst25vf_turn_off(); }

	return 1;
}

void sst25vf_ewsr() {
	runSingleCommand(EWSR);
}

void sst25vf_write_status_register(uint8_t status_data) {
	sst25vf_ewsr();
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	//SPI_FLUSH();
	spi_flush_buffer();
	clr_flash_cs();
	spi_write_byte(WRSR);
	spi_write_byte(status_data);
	//SPI_WRITE(WRSR);
	//SPI_WRITE(status_data);
	set_flash_cs();
}

void sst25vf_init() {
	spi_cs_init(SST25VF_CS_PORT_NUM, SST25VF_CS_PIN_NUM);
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	GPIO_SOFTWARE_CONTROL(SST25VF_FLASH_POWER_PORT_BASE, SST25VF_FLASH_POWER_PIN_MASK);
	GPIO_SET_OUTPUT(SST25VF_FLASH_POWER_PORT_BASE, SST25VF_FLASH_POWER_PIN_MASK);
	GPIO_SOFTWARE_CONTROL(SST25VF_FLASH_RESET_PORT_BASE, SST25VF_FLASH_RESET_PIN_MASK);
	GPIO_SET_OUTPUT(SST25VF_FLASH_RESET_PORT_BASE, SST25VF_FLASH_RESET_PIN_MASK);
	set_flash_power_pin();
	clr_flash_cs();
	clr_flash_reset_pin();
}

uint8_t sst25vf_4kb_erase(uint32_t addr) {
	uint8_t cmdBuffer[4];

	if(sst25vf_read_status_register() & STATUS_BUSY) {
		return 0;
	}

	shiftPageAddr(addr);
	cmdBuffer[0] = SECTOR_ERASE;
	cmdBuffer[1] = m_addr[0];
	cmdBuffer[2] = m_addr[1];
	cmdBuffer[3] = m_addr[2];

	enable_writes();
	runSpiByteTx(cmdBuffer, NULL, 0);

	status = SECTOR_ERASE_DONE;
	return 1;
}

uint8_t sst25vf_32kb_erase(uint32_t addr) {
	uint8_t cmdBuffer[4];

	if(sst25vf_read_status_register() & STATUS_BUSY) {
		return 0;
	}

	shiftPageAddr(addr);
	cmdBuffer[0] = SMALL_BLOCK_ERASE;
	cmdBuffer[1] = m_addr[0];
	cmdBuffer[2] = m_addr[1];
	cmdBuffer[3] = m_addr[2];

	enable_writes();
	runSpiByteTx(cmdBuffer, NULL, 0);

	status = SMALL_BLOCK_ERASE_DONE;
	return 1;
}

uint8_t sst25vf_64kb_erase(uint32_t addr) {
	uint8_t cmdBuffer[4];

	if(sst25vf_read_status_register() & STATUS_BUSY) {
		return 0;
	}

	shiftPageAddr(addr);
	cmdBuffer[0] = LARGE_BLOCK_ERASE;
	cmdBuffer[1] = m_addr[0];
	cmdBuffer[2] = m_addr[1];
	cmdBuffer[3] = m_addr[2];

	enable_writes();
	runSpiByteTx(cmdBuffer, NULL, 0);

	status = LARGE_BLOCK_ERASE_DONE;
	return 1;
}

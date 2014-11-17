#include "contiki.h"
#include "sst25vf.h"
#include "spi-arch.h"
#include "spi.h"
#include "dev/ssi.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/leds.h"
#include "sys/clock.h"

enum {PAGE_PROGRAM_DONE, CHIP_ERASE_DONE, SMALL_BLOCK_ERASE_DONE, PROGRAM_SID_DONE, IDLE} status = IDLE;
static uint32_t i = 0;
static uint8_t m_addr[3];

static struct ctimer ct;

static void (*page_program_callback)();
static void (*chip_erase_callback)();
static void (*program_sid_callback)();

void sst25vf_default_callback() {
	//leds_on(LEDS_ALL);
}

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


static inline void runSpiByteRx(uint8_t *cmdBuffer, void *rxBuffer, uint32_t rx_len) {
	SPI_FLUSH();
	clr_flash_cs();
	for(i = 0; i < 4; i++) {
		SPI_WRITE(cmdBuffer[i]);
	}
	for(i = 0; i < rx_len; i++) {
		SPI_READ(((uint8_t *)rxBuffer)[i]);
	}
	set_flash_cs();
}

static inline void runSpiByteTx(uint8_t *cmdBuffer, void *txBuffer, uint32_t tx_len) {
	SPI_FLUSH();
	clr_flash_cs();
	for(i = 0; i < 4; i++) {
		SPI_WRITE(cmdBuffer[i]);
	}
	for(i = 0; i < tx_len; i++) {
		SPI_WRITE(((uint8_t *)txBuffer)[i]);
	}
	set_flash_cs();
}

static inline void shiftPageAddr(uint32_t user_addr) {
	m_addr[0] = user_addr >> 16;
	m_addr[1] = user_addr >> 8;
	m_addr[2] = user_addr;
}

static inline void runSingleCommand(uint8_t cmd) {
	SPI_FLUSH();
	clr_flash_cs();
	SPI_WRITE(cmd);
	set_flash_cs();
}

static inline void wait_callback() {
	if(status == PAGE_PROGRAM_DONE) {
		(*page_program_callback)();
	}
	else if(status == CHIP_ERASE_DONE) {
		(*chip_erase_callback)();
	}
	else if(status == PROGRAM_SID_DONE) {
		(*program_sid_callback)();
	}

}

void sst25vf_set_program_callback(void *callback) {
	page_program_callback = callback;
}

void sst25vf_set_chip_erase_callback(void *callback) {
	chip_erase_callback = callback;
}

void sst25vf_set_program_sid_callback(void *callback) {
	program_sid_callback = callback;
}

void sst25vf_turn_on() {
	clr_flash_power_pin();
	clock_delay_usec(150);
}

void sst25vf_turn_off() {
	set_flash_power_pin();
	clr_flash_cs();
}

void sst25vf_read_page(uint32_t addr, void *rxBuffer, uint32_t rx_len) {
	uint8_t cmdBuffer[4];
	shiftPageAddr(addr);
	cmdBuffer[0] = READ;
	cmdBuffer[1] = m_addr[0];
	cmdBuffer[2] = m_addr[1];
	cmdBuffer[3] = m_addr[2];

	runSpiByteRx(&cmdBuffer[0], rxBuffer, rx_len);
}

void sst25vf_read_sid(uint8_t addr, void *rxBuffer, uint8_t rx_len) {
	uint8_t cmdBuffer[3];
	cmdBuffer[0] = READ_SID;
	cmdBuffer[1] = addr;
	cmdBuffer[2] = 0;
	SPI_FLUSH();
	clr_flash_cs();
	SPI_WRITE(cmdBuffer[0]);
	SPI_WRITE(cmdBuffer[1]);
	SPI_WRITE(cmdBuffer[2]);
	for(i = 0; i < rx_len; i++) {
		SPI_READ(((uint8_t *)rxBuffer)[i]);
	}
	set_flash_cs();
}

void sst25vf_program_sid(uint8_t addr, void *txBuffer, uint8_t tx_len) {
	uint8_t cmdBuffer[2];
	uint8_t check_status = 0;
	cmdBuffer[0] = PROGRAM_SID;
	cmdBuffer[1] = addr;
	SPI_FLUSH();
	sst25vf_write_enable();
	clr_flash_cs();
	SPI_WRITE(cmdBuffer[0]);
	SPI_WRITE(cmdBuffer[1]);
	for(i = 0; i < tx_len; i++) {
		SPI_WRITE(((uint8_t *)txBuffer)[i]);
	}
	set_flash_cs();

	status = PROGRAM_SID_DONE;
	ctimer_set(&ct, T_PROGRAM_SID, wait_callback, NULL);
}

uint8_t sst25vf_read_status_register() {
	uint8_t status_buffer = 0;
	SPI_FLUSH();
	clr_flash_cs();
	SPI_WRITE(RDSR);
	SPI_READ(status_buffer);
	set_flash_cs();

	return status_buffer;
}

void sst25vf_write_enable() {
	runSingleCommand(WREN);
}

void sst25vf_write_disable() {
	runSingleCommand(WRDI);
}

/*
	The write_enable command must be called prior to page program.
*/
void sst25vf_program(uint32_t addr,
				     void *txBuffer,
				     uint32_t tx_len) {
	uint8_t cmdBuffer[4];
	shiftPageAddr(addr);
	cmdBuffer[0] = PAGE_PROGRAM;
	cmdBuffer[1] = m_addr[0];
	cmdBuffer[2] = m_addr[1];
	cmdBuffer[3] = m_addr[2];
	sst25vf_write_enable();
	runSpiByteTx(&cmdBuffer[0], txBuffer, tx_len);

	status = PAGE_PROGRAM_DONE;
	ctimer_set(&ct, T_PAGE_PROGRAM, wait_callback, NULL);
}

void sst25vf_chip_erase() {
	uint8_t cmd = CHIP_ERASE;
	sst25vf_write_enable();
	runSingleCommand(cmd);
	status = CHIP_ERASE_DONE;
	ctimer_set(&ct, T_CHIP_ERASE, wait_callback, NULL);
}

void sst25vf_ewsr() {
	runSingleCommand(EWSR);
}

void sst25vf_wrsr(uint8_t status_data) {
	sst25vf_ewsr();
	SPI_FLUSH();
	clr_flash_cs();
	SPI_WRITE(WRSR);
	SPI_WRITE(status_data);
	set_flash_cs();
}

void sst25vf_init() {
	spi_cs_init(SST25VF_CS_PORT_NUM, SST25VF_CS_PIN_NUM);
	spi_set_mode(SSI_CR0_FRF_MOTOROLA, 0, 0, 8);
	sst25vf_set_chip_erase_callback(sst25vf_default_callback);
	sst25vf_set_program_callback(sst25vf_default_callback);
	sst25vf_set_program_sid_callback(sst25vf_default_callback);
	GPIO_SOFTWARE_CONTROL(SST25VF_FLASH_POWER_PORT_BASE, SST25VF_FLASH_POWER_PIN_MASK);
	GPIO_SET_OUTPUT(SST25VF_FLASH_POWER_PORT_BASE, SST25VF_FLASH_POWER_PIN_MASK);
	set_flash_power_pin();
	clr_flash_cs();
}

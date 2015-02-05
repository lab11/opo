#include "simplestore.h"


const static uint16_t max_page = 4095; // last page on chip. this is for the sst25vf064c.

static uint32_t write_head = 0;
static uint32_t read_head = 0;
bool empty = true;

PROCESS(simplestore_erase_process, "simplestore_erase_process");


void simplestore_turn_on_flash() {
	sst25vf_turn_on();
}

void simplestore_turn_off_flash() {
	sst25vf_turn_off();
}

uint8_t simplestore_write_next_page(void *txBuffer, uint8_t tx_len) {
	if(write_head < max_page) {
		sst25vf_program(write_head++ * 256, txBuffer, (uint32_t) tx_len);
		empty = false;
		return 1;
	}
	else {
		return 0; // chip full
	}
}

uint8_t simplestore_read_next_page(void *rxBuffer, uint8_t rx_len) {
	if(read_head == write_head) {
		return 0;
	}

	sst25vf_read_page(read_head++ * 256, rxBuffer, (uint32_t) rx_len);
	return 1;
}

void simplestore_clear_flash_chip() {
	write_head = 0;
	read_head = 0;
	empty = true;
	sst25vf_chip_erase();
}

bool simplestore_empty() {
	return empty;
}

void simplestore_set_flash_erase_callback(void *ecallback) {
	sst25vf_set_chip_erase_callback(ecallback);
}

void simplestore_set_flash_program_callback(void *pcallback) {
	sst25vf_set_program_callback(pcallback);
}

void simplestore_config() {
	uint32_t pc = 0;
	uint32_t counter = 0;
	uint8_t rbuf[256] = {0};
	uint16_t i = 0;
	for(pc=0;pc<max_page;pc++) {
		sst25vf_read_page(pc*256, &rbuf, 256);
		for(i=0;i<256;i++) {
			counter += rbuf[i];
		}
		if(counter == 256 * 256) {
			break;
		}
	}
	write_head = pc;
}


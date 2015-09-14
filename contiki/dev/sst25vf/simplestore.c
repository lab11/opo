#include "simplestore.h"

const static uint16_t max_page = 4095; // last page on chip. this is for the sst25vf064c.

static uint32_t write_head = 0;
static uint32_t read_head = 0;
static bool empty = true;
static bool on = false;

void simplestore_turn_on_flash() {
	on = true;
	sst25vf_turn_on();
}

void simplestore_turn_off_flash() {
	on = false;
	sst25vf_turn_off();
}

uint8_t simplestore_write_next_page(void *txBuffer, uint16_t tx_len) {
	if(write_head < max_page) {
		if(!sst25vf_program(write_head * 256, txBuffer, (uint32_t) tx_len)) {
			return SIMPLESTORE_FAIL;
		}
		write_head++;
		return SIMPLESTORE_SUCCESS;
	}
	else {
		return SIMPLESTORE_FULL; // chip full
	}
}

uint8_t simplestore_read_next_page(void *rxBuffer, uint16_t rx_len) {
	if(read_head == write_head || read_head >= max_page) {
		return SIMPLESTORE_READ_FULL; // Nothing more to be read.
	}

	if(!sst25vf_read_page(read_head * 256, rxBuffer, (uint32_t) rx_len)) {
		return SIMPLESTORE_FAIL; // Something went wrong with the SPI operation.
	}
	read_head++;
	return SIMPLESTORE_SUCCESS;
}

uint8_t simplestore_mark_last_page_done() {
	if(read_head == 0) { return SIMPLESTORE_FAIL; }
	uint8_t done = SIMPLESTORE_PAGE_DONE;
	sst25vf_program(read_head * 256 - 1, &done, 1);
	return SIMPLESTORE_SUCCESS;
}

uint8_t simplestore_roll_back_read() {
	if(read_head > 0) {
		read_head--;
		return SIMPLESTORE_SUCCESS;
	}
	else {
		return SIMPLESTORE_FAIL;
	}
}

uint8_t simplestore_clear_flash_chip() {
	write_head = 0;
	read_head = 0;
	empty = true;
	if(!sst25vf_chip_erase()) {
		return SIMPLESTORE_FAIL;
	}

	return SIMPLESTORE_SUCCESS;
}

bool simplestore_empty() {
	return empty;
}

uint32_t simplestore_pages_stored() {
	if(write_head == 0) {
		return 0;
	}
	if(read_head == 0) {
		return write_head;
	}
	return (write_head - read_head);
}

void simplestore_config() {
	uint32_t pc = 0; // page counter
	uint32_t counter = 0;
	uint8_t rbuf[256] = {0};
	uint16_t i = 0;

	sst25vf_turn_on();
	for(pc=0;pc<max_page;pc++) {
		sst25vf_read_page(pc*256, rbuf, 256);
		for(i=0;i<255;i++) { counter += rbuf[i]; }
		if(rbuf[255] == SIMPLESTORE_PAGE_DONE) {read_head = pc + 1;}
		if(counter == 255 * 255) { break; }
		counter = 0;
	}
	sst25vf_turn_off();
	write_head = pc;
}

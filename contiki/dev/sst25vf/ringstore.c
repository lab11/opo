#include "simplestore.h"


const static uint16_t max_page = 4095; // last page on chip. this is for the sst25vf064c.

static uint32_t tail = 0; // Ring buffer tail. Is a PAGE number.
static uint32_t head = 0; // Ring buffer head. Is a PAGE number.
static uint32_t pages_written = 0; // differentiate empty from full ring buffer
static uint32_t pages_read = 0;
static uint32_t read_head = 0;

PROCESS(simplestore_erase_process, "simplestore_erase_process");
static void (*user_erase_callback)();
static void (*user_program_callback)();
static void default_callback() {}

static void erase_callback() {
	process_poll(&simplestore_erase_process);
}

PROCESS_THREAD(simplestore_erase_process, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev==PROCESS_EVENT_POLL);
		if(pages_read == pages_written) {
			pages_written = 0;
			pages_read = 0;
			head = 0;
			tail = 0;
			read_head = 0;
			sst25vf_chip_erase();
		}
		else if(read_head > tail) {
			if(pages_read >= 256) {
				tail += 256;
				pages_read -= 256;
				pages_written -= 256;
				sst25vf_64kb_erase((tail-256) * 256);
			}
			else if(pages_read >= 128) {
				tail += 128;
				pages_read -= 128;
				pages_written -= 128;
				sst25vf_32kb_erase((tail-128) * 256);
			}
			else if(pages_read >= 16) {
				tail += 16;
				pages_read -= 16;
				pages_written -= 16;
				sst25vf_4kb_erase((tail-16) * 256);
			}
			else {
				(*user_erase_callback)();
			}
		}
		else if(read_head < tail) {
			// We've wrapped around
			if(tail >= max_page) {
				tail = 0;
				process_poll(&simplestore_erase_process);
			}
			else if(max_page - tail >= 256) {
				tail += 256;
				pages_read -= 256;
				pages_written -= 256;
				sst25vf_64kb_erase((tail-256) * 256);
			}
			else if(max_page - tail >= 128) {
				tail += 128;
				pages_read -= 128;
				pages_written -= 128;
				sst25vf_32kb_erase((tail-128) * 256);
			}
			else if(max_page - tail >= 16) {
				tail += 16;
				pages_read -= 16;
				pages_written -= 16;
				sst25vf_4kb_erase((tail-16) * 256);
			}
			else {
				(*user_erase_callback)();
			}
		}
		else {
			(*user_erase_callback)();
		}
	}
	PROCESS_END();
}

void simplestore_init() {
	sst25vf_set_chip_erase_callback(erase_callback);
	sst25vf_set_4kb_erase_callback(erase_callback);
	sst25vf_set_32kb_erase_callback(erase_callback);
	sst25vf_set_64kb_erase_callback(erase_callback);
	simplestore_set_flash_erase_callback(default_callback);
	process_start(&simplestore_erase_process, NULL);
}

void simplestore_turn_on_flash() {
	sst25vf_turn_on();
}

void simplestore_turn_off_flash() {
	sst25vf_turn_off();
}

uint8_t simplestore_write_next_page(void *txBuffer, uint8_t tx_len) {
	// Treat the chip as a ring buffer
	if(head == tail && pages_written == 0 && head < max_page) {
		sst25vf_program(head++ * 256, txBuffer, (uint32_t) tx_len);
	}
	else if(head > tail && head < max_page) {
		sst25vf_program(head++ * 256, txBuffer)
	}
	if(head <= max_page && (head > tail || pages_written == 0) ) {

		pages_written++;
		return 1; // good to go!
	}
	else if(head > max_page && tail > 0) {
		head = 0;
		sst25vf_program(head++ * 256, txBuffer, (uint32_t) tx_len);
		pages_written++;
		return 1; // good to go!
	}
	else {
		return 0; // chip is full :(
	}
}

uint8_t simplestore_read_next_page(void *rxBuffer, uint8_t rx_len) {
	if(pages_read == pages_written) {
		return 0; // nothing left to read
	}
	if(read_head < head) {
		pages_read++;
		sst25vf_read_page(read_head++ * 256, rxBuffer, (uint32_t) rx_len);
	}
	else if(read_head > head) {
		if(read_head <= max_page) {
			pages_read++;
			sst25vf_read_page(read_head++ * 256, rxBuffer, (uint32_t) rx_len);
		}
		else if(read_head > max_page) {
			pages_read++;
			read_head = 0;
			sst25vf_read_page(read_head++ * 256, rxBuffer, (uint32_t) rx_len);
		}
		else {
			return 0;
		}
	}

	return 1; // page read
}

uint8_t simplestore_roll_back_page() {
	// Tell simplestore to unmark the last read page as read
	if(read_head > tail) {
		read_head--;
		pages_read--;
		return 1;
	}
	else if(read_head < tail) {
		if(read_head > 0) {
			read_head--;
			pages_read--;
			return 1;
		}
		else if(read_head == 0) {
			read_head = max_page;
			pages_read--;
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0; // data already gone
	}
}

void simplestore_clear_flash_chip() {
	sst25vf_chip_erase(); // time efficient
}


void simplestore_clear_flash_read_pages() {
	process_poll(&simplestore_erase_process); // time inefficient
}

void simplestore_set_flash_erase_callback(void *ecallback) {
	user_erase_callback = ecallback;
}

void simplestore_set_flash_program_callback(void *pcallback) {
	user_program_callback = pcallback;
	sst25vf_set_program_callback(pcallback);
}

void simplestore_config() {
	uint32_t i = 0;
	uint32_t config_page_count = 0;
	uint32_t counter = 0;
	uint8_t  config_rbuf[256] = {0};
	bool     head_set = false;
	bool     tail_set = false;

	for(config_page_count=0;config_page_count<max_page;config_page_count++) {
		sst25vf_read_page(config_page_count*256, &config_rbuf, 256);

		for(i=0;i<256;i++) {
			counter += config_rbuf[i];
		}
		if(counter != 256 * 256) {
			if(!tail_set) {
				tail_set = true;
				tail = config_page_count;
			}
			pages_written += 1;
		}
		else if(counter == 256 * 256) {
			if(tail_set && !head_set) {
				head_set = true;
				head = config_page_count;
			}
		}
		if(head_set && tail_set) {
			break;
		}
		counter = 0;
		config_page_count++;
	}
}
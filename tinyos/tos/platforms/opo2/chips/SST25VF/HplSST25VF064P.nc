module HplSST25VF064C {
	provides {
		interface HplSST25VF064;
	}
	uses {
		interface Resource;
		interface SpiByte;
	}

}
implementatoin {

	command error_t read(uint8_t addr[3]) {

	}

	command error_t high_speed_read(uint8_t addr[3]) {}
	command error_t fast_read_dual_output(uint8_t addr[3] {}

	command error_t read_sid(uint8_t addr[3]) {

	}

	command error_t lock_sid() {

	}

	command error_t program_sid(uint8_t addr[3], uint8_t *data) {

	}

	command error_t read_status_register();

	command error_t write_enable();
	command error_t write_disable();

	command error_t page_program(uint8_t addr[3], uint8_t *data);
	command error_t dual_input_page_program(uint8_t addr[3], uint8_t *data);

	command error_t sector_erase(uint8_t addr[3]);
	command error_t 32kb_block_erase(uint8_t addr[3]);
	command error_t 64kb_block_erase(uint8_t addr[3]);
	command error_t chip_erase();

	command error_t ewsr(); // Enable write status register
	command error_t wrsr(uint8_t *data) // write status regsiter

	command error_t ehld(); // enable hold pin. turns reset pin into hold pin
	command error_t rdid(); // reads the manufacturer and device id


}
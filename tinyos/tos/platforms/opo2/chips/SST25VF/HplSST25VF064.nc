interface HplSST25VF064 {

	command void turnOn();
	command void turnOff();
	event void turnedOn();
	event void turnedOff();

	command void read(uint8_t addr[3], uint8_t *rxBuffer, uint16_t rx_len);
	event   void read_done(uint8_t *rxBuffer, uint16_t rx_len);

	command void high_speed_read(uint8_t addr[3], uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t len);
	command void fast_read_dual_output(uint8_t addr[3], uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t len);

	command void read_sid(uint8_t addr[3], uint8_t *rxBuffer, uint16_t rx_len);
	event   void read_sid_done(uint8_t *rxBuffer, uint16_t rx_len);

	command void lock_sid();
	command void program_sid(uint8_t addr[3], uint8_t *data, uint16_t len);

	command uint8_t read_status_register();

	command void write_enable();
	command void write_disable();

	command void page_program(uint8_t addr[3], uint8_t *txBuffer, uint16_t len);
	event   void page_program_done(uint8_t *txBuffer, uint16_t len);

	command void dual_input_page_program(uint8_t addr[3], uint8_t *txBuffer, uint16_t len);

	command void sector_erase(uint8_t addr[3]);
	command void small_block_erase(uint8_t addr[3]);
	command void large_block_erase(uint8_t addr[3]);

	command void chip_erase();
	event   void chip_erase_done();

	command void ewsr(); // Enable write status register
	command void wrsr(uint8_t data); // write status regsiter

	command void ehld(); // enable hold pin. turns reset pin into hold pin
	command void rdid(); // reads the manufacturer and device id


}
interface HplSST25VF064 {

	command void turnOn();
	command void turnOff();
	event   void turnedOn();
	event   void turnedOff();

	command void read(uint32_t addr, void *rxBuffer, uint32_t rx_len);
	event   void read_done(void *rxBuffer, uint32_t rx_len);

	command void page_program(uint32_t addr, void *txBuffer, uint32_t len);
	event   void page_program_done(void *txBuffer, uint32_t len);

	command void read_sid(uint8_t addr, void *rxBuffer, uint8_t rx_len);
	event   void read_sid_done(void *rxBuffer, uint8_t rx_len);

	command void lock_sid();
	command void program_sid(uint8_t addr, void *data, uint8_t len);
	event   void program_sid_done(void *data, uint8_t len);

	command uint8_t read_status_register();

	command void write_enable();
	command void write_disable();

	command void chip_erase();
	event   void chip_erase_done();

	command void ewsr(); // Enable write status register
	command void wrsr(uint8_t data); // write status regsiter

	command void ehld(); // enable hold pin. turns reset pin into hold pin
	command void rdid(); // reads the manufacturer and device id


}
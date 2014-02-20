interface HplAt45db {

	command void turnOn();
	command void turnOff();
	event   void turnedOn();
	event   void turnedOff();

	command void read(uint16_t page_addr, void *rxBuffer, uint16_t rx_len);
	event   void read_done(void *rxBuffer, uint16_t rx_len);

	command void write_buffer_1(void *b, uint16_t len);
	event   void write_buffer_1_done();
	command void write_buffer_2(void *b, uint16_t len);
	event   void write_buffer_2_done();

	command void flush_buffer_1(uint16_t page_addr);
	event   void flush_buffer_1_done();
	command void flush_buffer_2(uint16_t page_addr);
	event   void flush_buffer_2_done();

	command uint8_t* get_addr();
}
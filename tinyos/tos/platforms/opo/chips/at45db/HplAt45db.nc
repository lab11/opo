interface HplAt45dbs {

	command void turnOn();
	command void turnOff();
	event   void turnedOn();
	event   void turnedOff();

	command void read(uint8_t addr[3], uint8_t *rxBuffer, uint16_t rx_len);
	event   void read_done(uint8_t *rxBuffer, uint16_t rx_len);

	command void write_buffer_1(uint8_t *b, uint16_t len);
	event   void write_buffer_1_done();
	command void write_buffer_2(uint8_t *b, uint16_t len);
	event   void write_buffer_2_done();

	command void flush_buffer_1(uint16_t page_addr);
	event   void flush_buffer_1_done();
	command void flush_buffer_2(uint16_t page_addr);
	event   void flush_buffer_2_done();
}
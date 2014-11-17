#include "rv4162.h"
#include "dev/i2c.h"
#include <stdint.h>

uint8_t rv4162_slave_addr = RV4162_ADDR;
uint8_t rv4162_start_reg = 0;

uint8_t set_time_masks[8] = {
								0xFF, // Sub second
							    0x7F, // Second
							    0x7F, // Minute
							    0x7F, // Hour
							    0x07, // Day (Mon, Tue, etc)
							    0x3F, // Date (1-31)
							    0x1F, // Month (1-12)
							    0xFF  // Year (0-99), gonna be 20XX
							};

uint8_t rv4162_binary_to_bcd (uint8_t binary) {
	uint8_t out = 0;

	if (binary >= 40) {
	    out |= 0x40;
	    binary -= 40;
	}
	if (binary >= 20) {
	    out |= 0x20;
	    binary -= 20;
	}
	if (binary >= 10) {
	    out |= 0x10;
	    binary -= 10;
	}
	out |= binary;
	return out;
}

void rv4162_read_full_time(uint8_t *full_time) {
	uint8_t i = 0;
	i2c_init(I2C_SDA_PORT_NUM, I2C_SDA_PIN_NUM, I2C_SCL_PORT_NUM, I2C_SCL_PIN_NUM, I2C_SCL_NORMAL_BUS_SPEED);
	i2c_single_send(rv4162_slave_addr, rv4162_start_reg);
	i2c_master_set_slave_address(rv4162_slave_addr, I2C_RECEIVE);
	i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_START);
	while(i2c_master_busy()) {}
	for(i=0;i<8;i++) {
		full_time[i] = i2c_master_data_get();
		i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_CONT);
		while(i2c_master_busy()) {}
	}
	i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(i2c_master_busy()) {}
    for(i=0;i<8;i++) {
    	full_time[i] &= set_time_masks[i];
    	full_time[i] = RV4162_BCD_TO_BINARY(full_time[i]);
    }

}

/*

*/
void rv4162_set_time(uint8_t *full_time) {
	uint8_t write_buffer[9];
	uint8_t i = 0;
	write_buffer[0] = 0; // starting address
	for(i=0;i<8;i++) {
		write_buffer[i+1] = rv4162_binary_to_bcd(full_time[i]) & set_time_masks[i];
	}
	write_buffer[5] |= 0x10;
	i2c_init(I2C_SDA_PORT_NUM, I2C_SDA_PIN_NUM, I2C_SCL_PORT_NUM, I2C_SCL_PIN_NUM, I2C_SCL_NORMAL_BUS_SPEED);
	i2c_master_set_slave_address(rv4162_slave_addr, I2C_SEND);
	i2c_master_data_put(write_buffer[0]);
	i2c_master_command(I2C_MASTER_CMD_BURST_SEND_START);

	while(i2c_master_busy()) {}
	for(i=1;i<8;i++) {
		i2c_master_data_put(write_buffer[i]);
		i2c_master_command(I2C_MASTER_CMD_BURST_SEND_CONT);
		while(i2c_master_busy()) {}
	}
	i2c_master_data_put(write_buffer[8]);
	i2c_master_command(I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(i2c_master_busy()) {}
}
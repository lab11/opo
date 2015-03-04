#include "rv4162.h"
#include "dev/leds.h"

uint8_t rv4162_slave_addr = RV4162_ADDR;
uint8_t rv4162_start_reg = 0;

uint8_t set_time_masks[8] = {
								0xFF, // Sub second
							    0x7F, // Second
							    0x7F, // Minute
							    0x7F, // Hours
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

static time_t convert_to_unixtime(uint8_t *t) {
	time_t ut;
	struct tm cal_t;
	cal_t.tm_sec = t[1];
	cal_t.tm_min = t[2];
	cal_t.tm_hour = t[3];
	cal_t.tm_mday = t[5];
	cal_t.tm_mon = t[6] - 1;
	cal_t.tm_year = 2000 + (uint32_t)t[7] - 1900;
	ut = mktime(&cal_t);
	return ut;
}

void rv4162_read_full_time(uint8_t *full_time) {
	uint8_t i = 0;
	i2c_init(I2C_SDA_PORT_NUM, I2C_SDA_PIN_NUM, I2C_SCL_PORT_NUM, I2C_SCL_PIN_NUM, I2C_SCL_NORMAL_BUS_SPEED);
	i2c_single_send(rv4162_slave_addr, rv4162_start_reg);
	i2c_master_set_slave_address(rv4162_slave_addr, I2C_RECEIVE);
	i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_START);
	while(i2c_master_busy()) {}
	for(i=0;i<7;i++) {
		full_time[i] = i2c_master_data_get();
		i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_CONT);
		while(i2c_master_busy()) {}
	}
	full_time[7] = i2c_master_data_get();
	i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(i2c_master_busy()) {}
    for(i=0;i<8;i++) {
    	full_time[i] &= set_time_masks[i];
    	full_time[i] = RV4162_BCD_TO_BINARY(full_time[i]);
    }
}

time_t get_unixtime() {
	uint8_t temp[8] = {0};
	rv4162_read_full_time(&temp[0]);
	return convert_to_unixtime(&temp[0]);
}

void set_unixtime(time_t t) {
	struct tm *raw = gmtime(&t);
	uint8_t tbuf[8];
	tbuf[0] = 0; // subsecond
	tbuf[1] = (uint8_t) (raw->tm_sec);
	tbuf[2] = (uint8_t) (raw->tm_min);
	tbuf[3] = (uint8_t) (raw->tm_hour);
	tbuf[4] = 0;
	tbuf[5] = (uint8_t) (raw->tm_mday);
	tbuf[6] = (uint8_t) (raw->tm_mon + 1);
	tbuf[7] = (uint8_t) (raw->tm_year + 1900 - 2000);
	rv4162_set_time(&tbuf[0]);
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

void rv4162_disable_clkout() {
	uint8_t write_buffer[2] = {0x0A, 0};
	uint8_t i = 0;
	i2c_init(I2C_SDA_PORT_NUM, I2C_SDA_PIN_NUM, I2C_SCL_PORT_NUM, I2C_SCL_PIN_NUM, I2C_SCL_NORMAL_BUS_SPEED);
	i2c_master_set_slave_address(rv4162_slave_addr, I2C_SEND);
	i2c_master_data_put(write_buffer[0]);
	i2c_master_command(I2C_MASTER_CMD_BURST_SEND_START);
	while(i2c_master_busy()) {}
	i2c_master_data_put(write_buffer[i]);
	while(i2c_master_busy());
	i2c_master_command(I2C_MASTER_CMD_BURST_SEND_FINISH);
}

void rv4162_init() {
	set_unixtime(INIT_UNIXTIME);
}
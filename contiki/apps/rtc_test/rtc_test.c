#include "contiki.h"
#include "dev/i2c.h"
#include "RV4162.h"

PROCESS(rtc_test, "rtc-test");
AUTOSTART_PROCESSES(&rtc_test);

uint8_t full_time[9] = {0};
uint8_t read_buffer[11] = {0};
uint8_t i = 0;
uint8_t masks[8];


static struct etimer et;

PROCESS_THREAD(rtc_test, ev, data) {
	PROCESS_BEGIN();
	full_time[0] = 0;
	full_time[1] = 0;
    full_time[2] = 0;
    full_time[3] = 0;
    full_time[4] = 0;
    full_time[5] = 7;
    full_time[6] = 1;
    full_time[7] = 1;
    full_time[8] = 0;
    masks[0] = 0xFF;
    masks[1] = 0x7F;
    masks[2] = 0x7F;
    masks[3] = 0x7F;
    masks[4] = 0x07;
    masks[5] = 0x3F;
    masks[6] = 0xDF;
    masks[7] = 0xFF;

    for(i=0;i<8;i++) {
    	full_time[i+1] &= masks[i];
    }
    full_time[5] |= 0x10;

	i2c_init(I2C_SDA_PORT_NUM, I2C_SDA_PIN_NUM, I2C_SCL_PORT_NUM, I2C_SCL_PIN_NUM, I2C_SCL_NORMAL_BUS_SPEED);
	i2c_master_set_slave_address(RV4162_ADDR, I2C_SEND);
	i2c_master_data_put(full_time[0]);
	i2c_master_command(I2C_MASTER_CMD_BURST_SEND_START);

	while(i2c_master_busy()) {}
	for(i=1;i<8;i++) {
		i2c_master_data_put(full_time[i]);
		i2c_master_command(I2C_MASTER_CMD_BURST_SEND_CONT);
		while(i2c_master_busy()) {}
	}
	i2c_master_data_put(full_time[8]);
	i2c_master_command(I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(i2c_master_busy()) {}

	etimer_set(&et, CLOCK_SECOND * 5);

	while(1) {
		uint8_t start_addr = 0;
		uint8_t what_what = RV4162_ADDR;
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
		i2c_init(I2C_SDA_PORT_NUM, I2C_SDA_PIN_NUM, I2C_SCL_PORT_NUM, I2C_SCL_PIN_NUM, I2C_SCL_NORMAL_BUS_SPEED);
		i2c_single_send(what_what, start_addr);

		i2c_master_set_slave_address(RV4162_ADDR, I2C_RECEIVE);
		i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(i2c_master_busy()) {}
		for(i=0;i<7;i++) {
			read_buffer[i] = i2c_master_data_get();
			i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_CONT);
			while(i2c_master_busy()) {}
		}
		i2c_master_command(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
		while(i2c_master_busy()) {}
		read_buffer[7] = i2c_master_data_get();
		etimer_set(&et, CLOCK_SECOND * 5);
	}

	PROCESS_END();
}
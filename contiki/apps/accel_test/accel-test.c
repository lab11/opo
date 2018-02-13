#include "contiki.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "dev/leds.h"
#include "cloudcomm.h"
#include <stdbool.h>
#include "sys/clock.h"
#include "simplestore.h"
#include "vtimer.h"
#include "opo.h"
#include "adxl362.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "cpu.h"
#include "cc2538-rf-debug.h"
#include "vtimer.h"


#define CC2538_RF_CONF_CHANNEL 21

PROCESS(accel_test, "accel-test");
PROCESS(debug_proc, "debug proc");
PROCESS(accel_test_callback, "accel-test-callback");
AUTOSTART_PROCESSES(&accel_test);
count = 0;
static struct vtimer debugger;

static void debug_callback() {
	process_poll(&debug_proc);	
}

PROCESS_THREAD(debug_proc, ev, data) {
	PROCESS_BEGIN(); 
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		uint8_t status = accel_read_register(0x0B);
		char buffer[100];
		snprintf(buffer, 50, "Status: %u", status);
		send_rf_debug_msg(buffer);
		schedule_vtimer_ms(&debugger, 1000);
	}
	PROCESS_END();
}

static void accel_activity_callback() {
	GPIO_CLEAR_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);
	leds_toggle(LEDS_RED);
	char buffer[100];
	snprintf(buffer, 100, "WOOT + %u", count);
	send_rf_debug_msg(buffer);
	count += 1;
	process_poll(&accel_test_callback);
}

PROCESS_THREAD(accel_test_callback, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		//sst25vf_turn_on();
		accel_get_status();
		accel_write_register(0x27, 0x3F);
		accel_write_register(0xD, 0x0A);
		//sst25vf_turn_off();
	}
	PROCESS_END();
}

PROCESS_THREAD(accel_test, ev, data) {
	PROCESS_BEGIN();
	sst25vf_turn_on();
	char buffer[100];
	debugger = get_vtimer(debug_callback); 
	//spi_set_mode(0,0,0,8);

	spi_cs_init(ADXL362_CS_PORT_NUM, ADXL362_CS_PIN_NUM);
	GPIO_SOFTWARE_CONTROL(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_SET_INPUT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	ioc_set_over(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM, IOC_OVERRIDE_DIS);
	GPIO_DETECT_EDGE(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_DETECT_RISING(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_POWER_UP_ON_RISING(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_MASK);

	process_start(&accel_test_callback, NULL);
	process_start(&debug_proc, NULL);
	gpio_register_callback((gpio_callback_t) accel_activity_callback, ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);

	nvic_interrupt_enable(ADXL362_INTTWO_NVIC);
	GPIO_CLEAR_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);
	GPIO_ENABLE_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_ENABLE_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);

	GPIO_CLR_PIN(SST25VF_FLASH_POWER_PORT_BASE, SST25VF_FLASH_POWER_PIN_MASK);
	clock_delay_usec(10000); 
	clock_delay_usec(10000); 
	accel_soft_reset();
	clock_delay_usec(10000); 
	uint8_t dev_id = accel_read_register(0x00);
	uint8_t status = accel_read_register(0x0B);
	snprintf(buffer, 100, "Initial Stat, id: %u, %u", status, dev_id);
	send_rf_debug_msg(buffer);

	// set up activity threshold
	accel_write_register(0x20, 0x20);
	accel_write_register(0x21, 0);

	// set up inactivity threshold
	accel_write_register(0x23, 150);
	accel_write_register(0x24, 0);

	// Set up inactivity timer
	accel_write_register(0x25, 5);

 	// Config loop mode
	accel_write_register(0x27, 0x3F); // put into loop mode
	accel_write_register(0x2B, 0x40); // map int two to awake bit
	accel_write_register(0x2D, 0x0A); // setup the accelerometer

	// Check to see if there is a pending interrupt
	uint8_t awake = accel_read_register(0x0B) & 0x40;
	accel_write_register(0x2B, 0x40); 
	uint8_t inttwo = accel_read_register(0x2B);
	uint8_t inttow_pin = GPIO_READ_PIN(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	snprintf(buffer, 100, "Awake_bit + int + pin: %u %u %u", awake, inttwo, inttow_pin);
	send_rf_debug_msg(buffer);

	
	// If there is a pending interrupt
	if(awake != 0) {
		accel_read_register(0x0B);
		accel_write_register(0x27, 0x3F);
		accel_write_register(0x2D, 0x0A);
		accel_read_register(0x0B);
	} 
	//sst25vf_turn_off();
	schedule_vtimer_ms(&debugger, 1000);

	//leds_on(LEDS_BLUE);


	PROCESS_END();
}

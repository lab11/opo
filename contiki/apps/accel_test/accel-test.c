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


#define CC2538_RF_CONF_CHANNEL 21

PROCESS(accel_test, "accel-test");
PROCESS(accel_test_callback, "accel-test-callback");
AUTOSTART_PROCESSES(&accel_test);
count = 0;

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
		accel_get_status();
		accel_write_register(0x27, 0x3F);
		accel_write_register(0xD, 0x0A);
	}
	PROCESS_END();
}

PROCESS_THREAD(accel_test, ev, data) {
	PROCESS_BEGIN();
	spi_cs_init(ADXL362_CS_PORT_NUM, ADXL362_CS_PIN_NUM);
	GPIO_SOFTWARE_CONTROL(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_SET_INPUT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	ioc_set_over(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM, IOC_OVERRIDE_DIS);
	GPIO_DETECT_EDGE(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_TRIGGER_SINGLE_EDGE(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_DETECT_RISING(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_POWER_UP_ON_RISING(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_MASK);

	process_start(&accel_test_callback, NULL);
	gpio_register_callback((gpio_callback_t) accel_activity_callback, ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);

	accel_soft_reset();
	clock_delay_usec(10000); 
	// set up activity threshold
	accel_write_register(0x20, 0x20);
	accel_write_register(0x21, 0);

	// set up inactivity threshold
	accel_write_register(0x23, 150);
	accel_write_register(0x24, 0);

	// Set up inactivity timer
	accel_write_register(0x25, 5);

 	// Config loop mode
	accel_write_register(0x27, 0x3F);
	accel_write_register(0x2B, 0x40); // map inttwo to awake bit
	accel_write_register(0x2D, 0x0A);

	nvic_interrupt_enable(ADXL362_INTTWO_NVIC);
	GPIO_CLEAR_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_CLEAR_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);
	GPIO_ENABLE_INTERRUPT(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	GPIO_ENABLE_POWER_UP_INTERRUPT(ADXL362_INTTWO_PORT_NUM, ADXL362_INTTWO_PIN_NUM);

	uint8_t awake = accel_read_register(0x0B) & 0x40; 
	uint8_t inttwo = accel_read_register(0x2B);
	uint8_t inttow_pin = GPIO_READ_PIN(ADXL362_INTTWO_PORT_BASE, ADXL362_INTTWO_PIN_MASK);
	char buffer[100];
	snprintf(buffer, 100, "Awake_bit + pin: %u %u", awake, inttow_pin);
	send_rf_debug_msg(buffer);

	if(awake != 0) {
		accel_write_register(0x27, 0x3F);
		accel_write_register(0x2D, 0x0A);
	}

	//leds_on(LEDS_BLUE);


	PROCESS_END();
}

#include "nrf8001.h"
#include "contiki.h"
#include "sys/clock.h"
#include "spi-arch.h"
#include "spi.h"
#include "dev/ssi.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/leds.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "simple_network_driver.h"
#include "nrf8001_setup.h"
#include "rf_switch.h"

typedef void (* cloudcomm_callback_t)(void *data, uint16_t len);

typedef struct cloudcomm_metainfo {
	uint16_t ip[8];
} cloudcomm_meta_t;

typedef struct cloudcomm_data_packet {
	void *data;
	uint16_t len;
	uint16_t current_byte;
	struct cloudcomm_data_t *next;
} cloudcomm_data_t;

typedef struct cloudcomm_request_packet {
	uint8_t req;
	struct cloudcomm_req_t *next;
} cloudcomm_req_t;

void cloudcomm_init();
void start_cloudcomm();
void stop_cloudcomm();

void set_cloudcomm_metainfo(cloudcomm_meta_t metainfo);
void send_cloudcomm_data(void *data, size_t len);
void request_cloudcomm_data(uint8_t req);
void set_cloudcomm_rx_callback();

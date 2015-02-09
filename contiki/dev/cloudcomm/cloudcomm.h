#include "nrf8001.h"
#include "contiki.h"
#include "sys/clock.h"
#include "spi-arch.h"
#include "spi.h"
#include "dev/ssi.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/leds.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nrf8001_setup.h"
#include "rf_switch.h"
#include "simplestore.h"
#include "board.h"

#define CLOUDCOMM_REQ_TIME 			0
#define CLOUDCOMM_REQ_LOC			1
#define CLOUDCOMM_MAX_DATA_LENGTH	19
#define CLOUDCOMM_MAX_DATA_SIZE     60
#define CLOUDCOMM_DATA_QUEUE_LENGTH 50
#define CLOUDCOMM_REQ_QUEUE_LENGTH  2

typedef void (* cloudcomm_callback_t)(uint8_t packet[30], uint8_t len);

typedef struct cloudcomm_metainfo {
	uint16_t ip[8];
} cloudcomm_meta_t;

typedef struct cloudcomm_data_packet {
	uint8_t data[60];
	uint8_t len;
	uint8_t current_byte;
	struct cloudcomm_data_packet *next;
} cloudcomm_data_t;

void cloudcomm_init();
void start_cloudcomm();
void stop_cloudcomm();
void register_cloudcomm_callback(cloudcomm_callback_t callback, uint8_t req);

void set_cloudcomm_metainfo(cloudcomm_meta_t metainfo);
uint8_t send_cloudcomm_data(void *data, uint8_t len);
uint8_t request_cloudcomm_data(uint8_t req);
void set_cloudcomm_rx_callback();

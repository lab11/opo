#include "sst25vf.h"
#include "contiki.h"
#include "dev/leds.h"
#include <stdbool.h>

#define SIMPLESTORE_FULL		0
#define SIMPLESTORE_FAIL		1
#define SIMPLESTORE_SUCCESS		2

void simplestore_turn_on_flash();
void simplestore_turn_off_flash();

uint8_t simplestore_write_next_page(void *txBuffer, uint8_t len);
uint8_t simplestore_read_next_page(void *rxBuffer, uint8_t len);
uint8_t simplestore_roll_back_read();

uint8_t simplestore_clear_flash_chip();
void simplestore_config();
bool simplestore_empty();
uint32_t simplestore_pages_stored();
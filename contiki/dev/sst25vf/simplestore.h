#include "sst25vf.h"
#include "contiki.h"
#include "dev/leds.h"
#include <stdbool.h>

void simplestore_turn_on_flash();
void simplestore_turn_off_flash();

uint8_t simplestore_write_next_page(void *txBuffer, uint8_t len);
uint8_t simplestore_read_next_page(void *rxBuffer, uint8_t len);

void simplestore_clear_flash_chip();

void simplestore_set_flash_erase_callback(void *ecallback);
void simplestore_set_flash_program_callback(void *pcallback);
void simplestore_config();
bool simplestore_empty();
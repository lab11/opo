#include <stdint.h>

#ifndef BLE_TEST
#define BLE_TEST
#define CC2538_RF_CONF_CHANNEL 21

typedef struct opo_ble_unit {
    uint16_t tx_id;
    uint16_t rx_id;
    uint16_t ultrasonic_rf_dt;
    uint16_t ultrasonic_dt;
    uint32_t m_full_time;
    uint32_t tx_full_time;
    uint16_t m_reset_counter;
    uint16_t tx_reset_counter;
} opo_ble_t;


#endif
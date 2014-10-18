typedef struct opo_flash_storage_unit {
    uint16_t tx_id;
    uint16_t rx_id;
    uint16_t ultrasonic_rf_dt;
    uint32_t m_full_time;
    uint32_t tx_full_time;
    uint16_t m_reset_counter;
    uint16_t tx_reset_counter;
    uint16_t rx_fails;
} opo_sunit_t;

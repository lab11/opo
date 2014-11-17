#include <stdbool.h>
#include <stdint.h>

#ifndef OPO_H

#define OPO_H

#define OPO_PREAMBLE		38041

typedef struct opo_ranging_message {
	uint16_t preamble;
	uint16_t tx_id;
	uint8_t  time_confidence; // 0 = false time. 1 = believe it's true time
	uint8_t  tx_full_time[8];
    uint32_t tx_reset_counter;
    // Remaining stuff is for listening infrastructure
    uint16_t last_interaction_partner_id;
    uint16_t last_dt_ul_rf;
    uint8_t  last_full_time[8];
} opo_rmsg_t;

typedef struct opo_ranging_recived_info {
	uint16_t tx_id;
	uint8_t  time_confidence;
	uint8_t  tx_full_time[8];
    uint32_t tx_reset_counter;
} opo_rxmsg_t;

typedef void (* opo_rx_callback_t)(bool success, uint32_t ul_rf_dt, uint32_t ul_dt, opo_rxmsg_t rxmsg);

void setup_40kh_pwm(); // sets up pwm for transmit
void setup_comp1_timer();
void setup_comp2_timer(); // synced with comp1 timer
void setup_integrator_timer();
uint8_t enable_opo_rx();
uint8_t perform_opo_tx(opo_rmsg_t *msg);
void disable_opo_rx();
void register_opo_rx_callback(opo_rx_callback_t mcallback);
void register_opo_tx_callback(void *mcallback);
void opo_init();

#endif
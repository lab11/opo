#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifndef OPO_H
#define OPO_H

#ifndef OPO_ID
	#define OPO_ID 0
#endif

typedef struct __attribute__((packed)) opo_ranging_message {
	uint16_t preamble; // equal to the bitwise NOT of the id
	uint16_t id;       // Opo specific ID
	uint8_t  time_confidence; // 0 = false time. 1 = heard from smartphone, 2 = heard from node that heard from smartphone...
	time_t   unixtime;        // My unixtime
	// All the last_x stuff is data for listening infrastructure nodes
    uint16_t last_interaction_partner_id;
    time_t   last_unixtime;
    uint32_t last_range_dt;
    uint32_t ul_rf_dt;  // Time between when node sends UL pulse and TX SFD
} opo_rmsg_t;

// REORDERED FOR ALIGNMENT
typedef struct __attribute__((packed)) opo_storage_unit {
	uint16_t version_num; // In case we switch storage unit contents
	uint16_t rx_id;    // Opo specific rx id
	uint16_t tx_id;    // Opo specific tx id
	uint32_t range_dt; // Time diff betweent UL tx and UL rx
	uint32_t ul_dt;    // Time diff between two comparator triggers
	uint8_t  m_time_confidence; // 0 = false time. 1 = heard from smartphone. 2 = heard from node that heard from smartphone...
	time_t   m_unixtime;        // Unix time baby
	uint32_t failed_rx_count;   // High failed_rx count == Lower battery life
	// Stuff from transmitters
	time_t   tx_unixtime;
	uint8_t  tx_time_confidence;
	uint32_t ul_rf_dt; // Transmitted time diff between UL beacon and Radio packet
} opo_data_t;

typedef struct opo_metadata {
	uint16_t id;
	uint8_t  time_confidence;
} opo_meta_t;

typedef void (* opo_rx_callback_t)(opo_data_t odata);

void setup_40kh_pwm(); // sets up pwm for transmit
void setup_comp1_timer();
void setup_comp2_timer(); // synced with comp1 timer
void setup_integrator_timer();
uint8_t enable_opo_rx();
uint8_t perform_opo_tx();
void disable_opo_rx();
void register_opo_rx_callback(opo_rx_callback_t mcallback);
void register_opo_tx_callback(void *mcallback);
void opo_init();

#endif

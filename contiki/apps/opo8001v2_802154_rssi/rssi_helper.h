#pragma once 

typedef struct __attribute__((packed)) opo_storage_unit {
	uint16_t version_num; // In case we switch storage unit contents
	uint16_t rx_id;    // Opo specific rx id
	uint16_t tx_id;    // Opo specific tx id
	int rssi; 
	time_t   m_unixtime;        // Unix time baby
	// Stuff from transmitters
	time_t   tx_unixtime;
} opo_data_t;
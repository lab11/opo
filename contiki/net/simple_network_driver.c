#include "simple_network_driver.h"
#include "net/netstack.h"
#include "dev/leds.h"

/* 
Platform conf defines NETSTACK_CONF_NETWORK to point to simple_network_driver. 
NETSTACK_CONF_NETWORK gets defined into NETSTACK_NETWORK in contiki/core/net/netstakc.h
For opo, we define nullmac to NETSTACK_CONF_MAC and nullrdc to NETSTACK_CONF_RDC 
Okay, so in cc2538 rf core, the RX_TX ISR eventually calls NETSTACK_RDC.input, which calls 
NETSTACK_MAC.input, which calls NETSTACK_NETWORK.input, which finally calls simple_network_driver. 
*/ 

static void default_callback() {}
static void (*m_callback)() = default_callback;


static void init(void) {}

static void input(void) {
	(*m_callback)();
}

void simple_network_set_callback(void *callback) {
	m_callback = callback;
}

const struct network_driver simple_network_driver = {
	"Simple",
	init,
	input
};

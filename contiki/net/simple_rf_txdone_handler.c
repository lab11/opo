#include "simple_rf_txdone_handler.h"

static void default_callback() {}
static void (* m_callback)(void) = default_callback;

static void set_callback(void * c) {
	m_callback = c;
}

static void callback(void) {
	(*m_callback)();
}

struct rf_txdone_handler simple_rf_txdone_handler = {
	"SimpleRfTxDone",
	set_callback,
	callback
};
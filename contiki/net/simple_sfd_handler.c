#include "simple_sfd_handler.h"

static void (* m_callback)(void);

static void set_callback(void * c) {
	m_callback = c;
}

static void callback(void) {
	(*m_callback)();
}

struct sfd_handler simple_sfd_handler = {
	"SimpleSFD",
	set_callback,
	callback
};
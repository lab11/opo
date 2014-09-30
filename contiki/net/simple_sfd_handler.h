
#ifndef SIMPLE_SFD_HANDLER_H
#define SIMPLE_SFD_HANDLER_H

struct sfd_handler {
  char *name;

  /** Set callback function */
  void (* set_callback)(void *c);

  /** Callback for SFD trigger */
  void (* callback)(void);
};

extern struct sfd_handler simple_sfd_handler;
#endif
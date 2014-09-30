
#ifndef SIMPLE_RF_TXDONE_H
#define SIMPLE_RF_TXDONE_H

struct rf_txdone_handler {
  char *name;

  /** Set callback function */
  void (* set_callback)(void *c);

  /** Callback for SFD trigger */
  void (* callback)(void);
};

extern struct rf_txdone_handler simple_rf_txdone_handler;
#endif
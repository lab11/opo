DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
CONTIKI_PROJECT = simplestore-test

all: $(CONTIKI_PROJECT)
TARGETDIRS += ../../platform

SMALL = 0

CONTIKI = ../../../../contiki
include $(CONTIKI)/Makefile.include
CFLAGS += -DCC2538_RF_PRINT=1
DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
CONTIKI_PROJECT = randtest

all: $(CONTIKI_PROJECT)
TARGETDIRS += ../../platform

SMALL = 0

CONTIKI = /home/wwhuang/Dropbox/git/contiki
include $(CONTIKI)/Makefile.include
CFLAGS += -DINIT_UNIXTIME=1436579198
CFLAGS += -DOPO_ID=2
CFLAGS += -DCC2538_RF_PRINT=1
CFLAGS += -DRF_DEBUG_ID=1

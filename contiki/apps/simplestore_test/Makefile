DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
CONTIKI_PROJECT = simplestore-test

all: $(CONTIKI_PROJECT)
TARGETDIRS += ../../platform

SMALL = 0

CONTIKI = ../../../../contiki
include $(CONTIKI)/Makefile.include
CFLAGS += -DINIT_UNIXTIME=1424460031
CFLAGS += -DOPO_ID=1
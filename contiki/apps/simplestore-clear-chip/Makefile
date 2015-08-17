DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
CONTIKI_PROJECT = simplestore-clear

TARGETDIRS += ../../platform/

all: $(CONTIKI_PROJECT)

CONTIKI = ../../../../contiki
include $(CONTIKI)/Makefile.include
CFLAGS += -DINIT_UNIXTIME=1424460031
CFLAGS += -DOPO_ID=1
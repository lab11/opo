import sh
import datetime
import sys
import csv
import arrow

if len(sys.argv) != 2:
	print "need id!"
	sys.exit()

m_id = int(sys.argv[1])

today = arrow.utcnow()

rmake = open("Makefile.template", 'rb')
wmake = open("Makefile", 'wb')

for line in rmake:
	wmake.write(line)

base = "CFLAGS += -D{0}={1}\n"
wmake.write(base.format('INIT_UNIXTIME', today.timestamp))
wmake.write(base.format('OPO_ID', m_id))
wmake.write(base.format("CC2538_RF_PRINT",0))
wmake.write(base.format("RF_DEBUG_ID", m_id))

wmake.close()
sh.make("clean")
m = sh.make("opo8001rxtx.upload")
print m.stdout
print m.stderr

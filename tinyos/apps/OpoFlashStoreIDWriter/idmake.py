import sh
import shutil
import sys
import os

if len(sys.argv) != 3:
	print "need id and port"
	sys.exit()

r = open('node_info.data', 'rb')
rmake = open('Makefile.template', 'rb')
wmake = open('Makefile', 'wb')
mid = int(sys.argv[1])
port = sys.argv[2]

r.next()

for line in rmake:
	wmake.write(line)

for line in r:
	raw = line.strip().split()
	i = int(raw[0].strip())
	s = int(raw[1].strip())
	if i == mid:
		base = "CFLAGS += -D{0}={1}\n"
		wmake.write(base.format("M_ID", str(i)))
		wmake.write(base.format("M_SEED", str(s)))
		break

wmake.close()
m = sh.make("opo", "install", "bsl,/dev/" + port)
print m.stdout
print m.stderr

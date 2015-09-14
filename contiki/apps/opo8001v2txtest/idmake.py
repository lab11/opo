import sh
import sys
import fileinput

if len(sys.argv) != 2:
	print "need id!"
	sys.exit()

m_id = int(sys.argv[1])

for line in fileinput.input("../../dev/opo/opo.c", inplace=1):
	if "#define OPO_ID" in line:
		sys.stdout.write("#define OPO_ID " + str(m_id) + "\n")
	else:
		sys.stdout.write(line)

fileinput.close()

m = sh.make("opo8001rxtx.upload")
print m.stdout
print m.stderr

for line in fileinput.input("opo8001rxtx.c", inplace=1):
	if "#define OPO_ID" in line:
		sys.stdout.write("#define OPO_ID 0\n")
	else:
		sys.stdout.write(line)
fileinput.close()

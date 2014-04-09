import sh
import datetime
import sys
import csv

if len(sys.argv) != 3:
	print "need id and port"
	sys.exit()

r = open('node_info.data', 'rb')
m_id = int(sys.argv[1])
m_seed = 0
port = sys.argv[2]

r.next()
for line in r:
	raw = line.strip().split()
	i = int(raw[0].strip())
	s = int(raw[1].strip())
	if i == m_id:
		m_id = m_id
		m_seed = s
		break



#Wait this just converts to hex
def getBCDRep(t):
	bcd_rep = 0
	time_remaining = t
	if time_remaining >= 40:
		bcd_rep |= 0b01000000
		time_remaining -= 40
	if time_remaining >= 20:
		bcd_rep |= 0b00100000
		time_remaining -= 20
	if time_remaining >= 10:
		bcd_rep |= 0b00010000
		time_remaining -= 10
	if time_remaining >= 8:
		bcd_rep |= 0b00001000
		time_remaining -= 8
	if time_remaining >= 4:
		bcd_rep |= 0b00000100
		time_remaining -= 4
	if time_remaining >= 2:
		bcd_rep |= 0b00000010
		time_remaining -= 2
	if time_remaining >= 1:
		bcd_rep |= 0b00000001
		time_remaining -= 1

	return bcd_rep

r.next()

today = datetime.datetime.today()
year = getBCDRep(today.year - 2000)
month = getBCDRep(today.month)
date = getBCDRep(today.day)
hour = getBCDRep(today.hour)
minute = getBCDRep(today.minute)
sec = getBCDRep(today.second)

weekday = today.isoweekday() + 1
if weekday == 8:
	weekday = 1

weekday = getBCDRep(weekday)

positions = ["M_SEC", "M_MIN", "M_HOUR", "M_WEEKDAY", "M_DATE", "M_MONTH", "M_YEAR"]
time_info = [sec, minute, hour, weekday, date, month, year]

rmake = open("Makefile.template", 'rb')
wmake = open("Makefile", 'wb')

for line in rmake:
	wmake.write(line)

base = "CFLAGS += -D{0}={1}\n"
for i in range(len(positions)):
	wmake.write(base.format(positions[i], time_info[i]))

wmake.write(base.format('M_SEED', m_seed))
wmake.write(base.format('M_ID', m_id))
wmake.close()
m = sh.make("opo2", "install." + str(m_id), "bsl,/dev/" + port)
print m.stdout
print m.stderr


import sh
import shutil
import datetime
import os

today = datetime.datetime.today()

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

month = getBCDRep(today.month)
date = getBCDRep(today.day)
minute = getBCDRep(today.minute)
sec = today.second
weekday = today.weekday()



shutil.move("OpoFlashStoreP.nc", "OpoFlashStoreP.nc.bak")

reader = open("OpoFlashStoreP.nc.bak", 'rb')
writer = open("OpoFlashStoreP.nc", 'wb')

for line in reader:
	if line.strip() == "uint8_t initial_time[8];":
		print line

	writer.write(line)

os.remove("OpoFlashStoreP.nc.bak")
#sh.make("opo", "install", "bsl,/dev/ttyUSB0")
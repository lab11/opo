import csv
import sys
import time

m_file = open('visual.csv', 'wb')
writer = csv.writer(m_file, delimiter=' ')
throwaway = sys.stdin.readline()
receivers = {}
transmitters = {}
nodes = 6

while True:
	line = sys.stdin.readline()
	s = line.strip().split()
	if len(s) == 10:
		receivers[s[1]] = 0
		transmitters[s[2]] = 0

	if len(receivers) == nodes and len (transmitters) == nodes:
		print "Ready!"
		break
	else:
		print "rx: " + str(len(receivers)) + " tx: " + str(len(transmitters))


while True:
	line = sys.stdin.readline()
	current_time = time.mktime(time.gmtime())
	s = line.strip()
	s = s.split()
	s.append(current_time)
	writer.writerow(s)
	m_file.flush()
	print s
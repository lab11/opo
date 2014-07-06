import csv
import glob
import json
import ast
import sys
import datetime
import time
import scipy as sp

outfile = open('aggregated.dat', 'wb')
basefile = open('historical_opo_deduped.dat', 'rb')

fnames = glob.glob("*.csv")

readers = []

for f in fnames:
	r = csv.reader(open(f, 'rb'), delimiter=' ')
	readers.append(r)

for line in basefile:
	d = ast.literal_eval(line)

	lsec = d['last_full_time'][1]
	lminute = d['last_full_time'][2]
	lhr = d['last_full_time'][3]
	ld = d['last_full_time'][5] & 0x3f
	lmonth = d['last_full_time'][6]
	lyear = d['last_full_time'][7]
	l_date = datetime.datetime(lyear, lmonth, ld, lhr, lminute, lsec)

	if ld == 0:
		ld = 1
	if lmonth == 0:
		lmonth = 1

	l_date = datetime.datetime(lyear, lmonth, ld, lhr, lminute, lsec)
	l_true_date = datetime.datetime.fromtimestamp(int(d['interaction_time']))
	print woo


for r in reader:
	for line in r:

		m_id = int(r[1])
		o_id = int(r[2])
		ul_rf_dt = int(r[3])

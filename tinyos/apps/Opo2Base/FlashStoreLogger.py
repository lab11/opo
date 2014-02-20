import sys
import urllib2
import json
import time
import datetime
import csv

now = datetime.datetime.now()
throwaway = sys.stdin.readline()

m_file = open('flash_store-' + now.strftime("%c").replace(" ", "-") + '.csv', 'wb')
writer = csv.writer(m_file, delimiter=' ')

while True:
    packet = {}

    line = sys.stdin.readline()
    print line
    m_file.write(line + "\n")
    m_file.flush()

    """
    check = line.strip().split()
    m_file.write
    writer.writerow(line)

    if(check[0].isdigit()):
        print
        s = line.strip().split()

        packet['rx_id'] = int(s[3], 16)
        packet['tx_id'] = int(s[4], 16)
        packet['range'] = (float(s[2]) - float(s[1])) / 32000 * 340.29 - .08
        packet['time'] = time.mktime(time.gmtime())
        packet['rtc_time'] = int(s[5])
        print packet
        writer.writerow(s)
        m_file.flush()
        data = json.dumps(packet)
        d_len = len(data)
    """
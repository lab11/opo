import sys
import urllib2
import json
import time
import csv

throwaway = sys.stdin.readline()
current_time = time.mktime(time.gmtime())
m_file = open('/home/wwhuang/visual_out-' + str(current_time) + '.csv', 'wb')
writer = csv.writer(m_file, delimiter=' ')

while True:
    packet = {}

    line = sys.stdin.readline()
    check = line.strip().split()
    if(check[0].isdigit()):
        print line
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

        url = 'http://opo.fusion.eecs.umich.edu/receive_data'
        req = urllib2.Request(url, data, {'Content-Type': 'application/json',
                                          'Content-Length': d_len})
        try:
            r = urllib2.urlopen(req)
        except urllib2.HTTPError, e:
            print "HTTP ERROR"
            print "Code: " + str(e.code)
            print "Msg: " + str(e.msg)
            print "Hdrs: " + str(e.hdrs)
            print "fp: " + str(e.fp)
        except:
            print "unknown exception", sys.exc_info()[0]

#!/usr/bin/env python2

import IPy
import json
import sys
import copy

if len(sys.argv) == 2:
	verbose = True
elif len(sys.argv) == 1:
	verbose = False
else:
	raise NotImplementedError('real argument parsing. deal wit it.')

try:
	import socketIO_client as sioc
except ImportError:
	print('Could not import the socket.io client library.')
	print('sudo pip install socketIO-client')
	sys.exit(1)

try:
	import termcolor
except ImportError:
	print('Could not import the termcolor library')
	print('sudo pip install termcolor')

import logging
logging.basicConfig()

SOCKETIO_HOST      = 'inductor.eecs.umich.edu'
SOCKETIO_PORT      = 8082
SOCKETIO_NAMESPACE = 'stream'

query = {'profile_id': '4wbddZCSIj'}

pkts = {}
names = {}

dup_cnt = {}

class stream_receiver (sioc.BaseNamespace):
	def on_reconnect (self):
		if 'time' in query:
			del query['time']
		stream_namespace.emit('query', query)

	def on_connect (self):
		stream_namespace.emit('query', query)

	def on_data (self, *args):
		pkt = args[0]
		orig_pkt = copy.deepcopy(pkt)
		del pkt['time']
		del pkt['seq']
		del pkt['_id']
		del pkt['true_full_time']
		del pkt['last_full_time']
		del pkt['m_full_time']
		spkt = json.dumps(pkt)
		#print(spkt)

		try:
			names[pkt['last_heard_id']] = pkt['full_name'].split(' ')[0]
		except KeyError:
			pass

		try:
			pkts[spkt] += 1
		except KeyError:
			pkts[spkt] = 1
		if verbose:
			print("Count: {}. Packet: {}".format(pkts[spkt], orig_pkt))
		else:
			sys.stdout.write(' '*120 + '\r')
			if pkts[spkt] == 1:
				dup_cnt[pkt['last_heard_id']] = 0
				# id --> last_heard_id
				try:
					print('{:1.2f}m {} --> {}'.format(pkt['range'],
						names[pkt['id']], names[pkt['last_heard_id']]))
				except KeyError:
					print('{:1.2f}m {} --> {}'.format(pkt['range'],
						pkt['id'], pkt['last_heard_id']))
			else:
				try:
					dup_cnt[pkt['last_heard_id']] += 1
				except KeyError:
					dup_cnt[pkt['last_heard_id']] = 1

			ids = dup_cnt.keys()
			ids.sort()
			s = ''
			c = 0
			for i in ids:
				c += dup_cnt[i]
				try:
					t = '{}: {} '.format(names[i], dup_cnt[i])
				except KeyError:
					t = '{}: {} '.format(i, dup_cnt[i])
				if i == pkt['last_heard_id']:
					s += termcolor.colored(t, color='yellow', attrs=['bold'])
				else:
					s += t
			if c:
				sys.stdout.write('Suppressed {} duplicate(s). {}\r'.format(c, s))
				sys.stdout.flush()


socketIO = sioc.SocketIO(SOCKETIO_HOST, SOCKETIO_PORT)
stream_namespace = socketIO.define(stream_receiver,
	'/{}'.format(SOCKETIO_NAMESPACE))

socketIO.wait()

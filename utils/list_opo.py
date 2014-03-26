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

import logging
logging.basicConfig()

SOCKETIO_HOST      = 'inductor.eecs.umich.edu'
SOCKETIO_PORT      = 8082
SOCKETIO_NAMESPACE = 'stream'

query = {'profile_id': '4wbddZCSIj'}

pkts = {}
names = {}

dup_cnt = 0

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
		del pkt['last_seq']
		del pkt['_id']
		spkt = json.dumps(pkt)

		try:
			names[pkt['tx_id']] = pkt['name']
		except KeyError:
			pass

		try:
			pkts[spkt] += 1
		except KeyError:
			pkts[spkt] = 1
		if verbose:
			print("Count: {}. Packet: {}".format(pkts[spkt], pkt))
		else:
			global dup_cnt
			sys.stdout.write(' '*40 + '\r')
			if pkts[spkt] == 1:
				dup_cnt = 1
				# last_tx_id --> tx_id
				try:
					print('{:1.2f}m {} --> {}'.format(pkt['range'],
						names[pkt['last_tx_id']], names[pkt['tx_id']]))
				except KeyError:
					print('{:1.2f}m {} --> {}'.format(pkt['range'],
						pkt['last_tx_id'], pkt['tx_id']))
			else:
				dup_cnt += 1
				sys.stdout.write('Suppressed {} duplicate(s)\r'.format(dup_cnt))
				sys.stdout.flush()


socketIO = sioc.SocketIO(SOCKETIO_HOST, SOCKETIO_PORT)
stream_namespace = socketIO.define(stream_receiver,
	'/{}'.format(SOCKETIO_NAMESPACE))

socketIO.wait()

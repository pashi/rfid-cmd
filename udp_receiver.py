#!/usr/bin/python

# listen udp port and print code if it valid syntax

import socket
import traceback
import string
import mosquitto
import time
import thread
import json

data = {}


def validate_input(message):
    n = int(message)
    if n < 1:
        raise ValueError('too small value')
    if not len(message) == 10:
        raise ValueError('invalid lenght')
    return n


def mtqq_send(m, name, val):
    m.connect('localhost')
    message = str(val).encode('string_escape')
    m.publish(name, message)
    m.loop(2)
    m.disconnect()


def open_door(m):
    time.sleep(1)
    mtqq_send(m, "action/door/open", "open")
    time.sleep(10)
    mtqq_send(m, "action/door/open", "close")

host = ''
port = 8888

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((host, port))
m = mosquitto.Mosquitto("udp_client")

with open('tokens.json', 'r') as f:
    data = json.load(f)

while 1:
    try:
        message, address = s.recvfrom(8192)
        validate_input(message)
        #print "Got data from", address
        #print message
	
        if data.has_key(message):
            name = data[message]
            mtqq_send(m, "auth/accept/name", name)
            thread.start_new_thread(open_door, (m,))
        else:
            mtqq_send(m, "auth/reject/unknown", "unknown")
    except (KeyboardInterrupt, SystemExit):
        raise
    except:
        # pass
        traceback.print_exc()

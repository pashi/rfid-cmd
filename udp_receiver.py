#!/usr/bin/python

# listen udp port and print code if it valid syntax

import socket, traceback
import string


def validate_input(message):
    n = int(message)
    if n < 1:
        raise ValueError('too small value')
    if not len(message) == 10:
        raise ValueError('invalid lenght')
    return n

host = ''
port = 8888

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((host, port))

while 1:
    try:
        message, address = s.recvfrom(8192)
	validate_input(message)
        print "Got data from", address
	print message
    except (KeyboardInterrupt, SystemExit):
        raise
    except:
        pass
        #traceback.print_exc()


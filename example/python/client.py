#!/usr/bin/python
# -*- coding: utf-8 -*-

from zmq import Context, REQ

def Main():
    context = Context()
    print("Connecting to hello world server...")
    socket = context.socket(REQ)
    socket.connect("udp://localhost:15555")

    for i in range(10):
        socket.send(b"Hello")
        message = socket.recv()
        print("Received reply %s [ %s ]" % (i, message))

if __name__ == '__main__':
    Main()
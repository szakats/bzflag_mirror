#!/usr/bin/env python
from BZFlag import Server

server = Server.BaseServer(interface = '')

print "Server is listening on %s:%d" % (server.tcp.interface, server.tcp.port)

def onConnect(socket):
    print "Connection from %s:%s" % (socket.address[0], socket.address[1])
server.onConnect.observe(onConnect)

def onDisconnect(socket):
    print "Disconnection from %s:%s" % (socket.address[0], socket.address[1])
server.onDisconnect.observe(onDisconnect)

server.run()

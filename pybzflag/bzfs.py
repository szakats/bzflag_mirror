#!/usr/bin/env python
from BZFlag import CommandLine

server = CommandLine.server()

print "Server is listening on %s:%d" % (server.tcp.interface, server.tcp.port)

def onConnect(socket):
    print "Connected client %d, from %s:%s" % (socket.id, socket.address[0], socket.address[1])
server.onConnect.observe(onConnect)

def onDisconnect(socket):
    print "Disconnected client %d, from %s:%s" % (socket.id, socket.address[0], socket.address[1])
server.onDisconnect.observe(onDisconnect)

server.run()

#!/usr/bin/env python
from BZFlag import CommandLine

server = CommandLine.server()

print "Server is listening on %s:%d" % (server.tcp.interface, server.tcp.port)

server.onConnect.trace(lambda socket: "Connected client %d, from %s:%s" %
                       (socket.id, socket.address[0], socket.address[1]))
server.onDisconnect.trace(lambda socket: "Disconnected client %d, from %s:%s" %
                          (socket.id, socket.address[0], socket.address[1]))

server.run()

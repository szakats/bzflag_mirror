#!/usr/bin/env python
#
# A utility that acts as a BZFlag proxy server, showing all
# messages in a human readable form.
#
from BZFlag import CommandLine, Server, Client, Event

# Create a server and a client, sharing command line options and event loop
eventLoop = Event.EventLoop()
(server, client) = CommandLine.Parser([Server.BaseServer, Client.BaseClient],
                                      eventLoop = eventLoop
                                      ).parse()

# Trace server connections and disconnections
print "Server is listening on %s:%d" % (server.tcp.interface, server.tcp.port)
server.onConnect.trace(lambda socket: "Connected client %d, from %s:%s" %
                       (socket.id, socket.address[0], socket.address[1]))
server.onDisconnect.trace(lambda socket: "Disconnected client %d, from %s:%s" %
                          (socket.id, socket.address[0], socket.address[1]))

# Show client connection status
client.onConnect.trace("Connected to server %s" % client.options['server'])

eventLoop.run()

#!/usr/bin/env python
from BZFlag import CommandLine

server = CommandLine.server()

# Trace connections and disconnections
print "Server is listening on %s:%d" % (server.tcp.interface, server.tcp.port)
server.onConnect.trace("Connected %(1)s")
server.onDisconnect.trace("Disconnected %(1)s")

server.run()

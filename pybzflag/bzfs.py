#!/usr/bin/env python
from BZFlag import CommandLine, Server

argParser = CommandLine.Parser(Server.StandardServer)
argParser.add_option("-v", "--view", dest="view", action="store_true", 
                     help="Shows an overhead view of the game in progress")
server = argParser.parse()

# Trace connections and disconnections
print "Server is listening on %s:%d" % (server.tcp.interface, server.tcp.port)
server.onConnect.trace("Connected %(1)s")
server.onDisconnect.trace("Disconnected %(1)s")

if server.cmdLineValues['view']:
    from BZFlag.UI import OverheadView
    OverheadView.attach(server.game, server.eventLoop)

server.run()

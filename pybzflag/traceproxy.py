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

# Let the user know when our client connects, and force
# our server to hand out the same client ID we were given.
def onClientConnect():
    print "Connected to server %s" % client.options['server']
    print "Client ID is %d" % client.id
    server.nextClientID = client.id   
client.onConnect.observe(onClientConnect)

# Dump a message contents to stdout. It should already be
# mostly human readable, thanks to the Protocol module.
def dumpMessage(msg, direction):
    print "\n%s %s" % (direction, msg.__class__.__name__)
    for key in msg.__dict__:
        if key[0] != '_' and not key in ('eventLoop', 'socket', 'header'):
            print "%15s: %r" % (key, msg.__dict__[key])

# Set up events to forward messages between
# client and server, dumping them to stdout
def onClientMessage(msg):
    server.tcp.write(msg)
    dumpMessage(msg, '<--')
    return 1
client.onAnyMessage.observe(onClientMessage)
def onServerMessage(msg):
    client.tcp.write(msg)
    dumpMessage(msg, '-->')
    return 1
server.onAnyMessage.observe(onServerMessage)

eventLoop.run()

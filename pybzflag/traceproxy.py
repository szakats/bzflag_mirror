#!/usr/bin/env python
#
# A utility that acts as a BZFlag proxy server, showing all
# messages in a human readable form.
#
from BZFlag import CommandLine, Server, Client, Event, Protocol, Util
import sys

# Create a server and a client, sharing command line options and event loop
eventLoop = Event.EventLoop()
argParser = CommandLine.Parser([Server.BaseServer, Client.BaseClient],
                               eventLoop = eventLoop)
argParser.add_option("-n", "--names", action="store_true", dest="names",
                     help="Disables the output of message contents, shows only their names.")
argParser.add_option("-d", "--hide", action="append", dest="hide",
                     help="Hides the given message type.")
argParser.add_option("-o", "--show", action="append", dest="show",
                     help="Shows only the given message type.")
(server, client) = argParser.parse()
options = client.cmdLineValues


# Trace server connections and disconnections
print "--- Server is listening on %s:%d" % (server.tcp.interface, server.tcp.port)
server.onConnect.trace(lambda socket: "*** Connected client %d, from %s:%s" %
                       (socket.id, socket.address[0], socket.address[1]))
server.onDisconnect.trace(lambda socket: "*** Disconnected client %d, from %s:%s" %
                          (socket.id, socket.address[0], socket.address[1]))


# Let the user know when our client connects, and force
# our server to hand out the same client ID we were given.
def onClientConnect():
    print "*** Connected to server %s" % client.options['server']
    print "--- Client ID is %d" % client.id
    server.nextClientID = client.id
    server.clientIDIncrement = 0
client.onConnect.observe(onClientConnect)

# Dump the message using Util.messageDump, filtering it according to our options
def showMessage(msg):
    name = msg.__class__.__name__
    if options['hide'] and name in options['hide']:
        return
    if options['show'] and not name in options['show']:
        return
    Util.messageDump(msg, sys.stdout, not options['names'])

# Set up events to forward messages between
# client and server, dumping them to stdout
def onClientMessage(msg):
    try:
        server.clientsByID[client.id].write(msg, msg.protocol)
        showMessage(msg)
    except KeyError:
        pass
client.onUnhandledMessage.replace(onClientMessage)

def onServerMessage(msg):
    getattr(client, msg.protocol).write(msg)
    showMessage(msg)
server.onUnhandledMessage.replace(onServerMessage)

eventLoop.run()

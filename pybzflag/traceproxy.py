#!/usr/bin/env python
#
# A utility that acts as a BZFlag proxy server, showing all
# messages in a human readable form.
#
from BZFlag import CommandLine, Server, Client, Event, Protocol
from StringIO import StringIO


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
client.onConnect.observe(onClientConnect)


# Dump a message contents to stdout. It should already be
# mostly human readable, thanks to the Protocol module.
def dumpMessage(msg, direction):
    name = msg.__class__.__name__

    # A hex dump utility used to format 'data' fields
    def hexDump(value, bytesPerLine=16, wordSize=2):
        src = StringIO(value)
        dest = StringIO()
        addr = 0
        while 1:
            srcLine = src.read(bytesPerLine)
            if not srcLine:
                break

            # Address
            dest.write("%04X: " % addr)
            addr += len(srcLine)

            # Hex values
            for i in xrange(bytesPerLine):
                if i < len(srcLine):
                    dest.write("%02X" % ord(srcLine[i]))
                else:
                    dest.write("  ")
                if not (i+1) % wordSize:
                    dest.write(" ")
            dest.write(" ")

            # ASCII representation
            for byte in srcLine:
                if ord(byte) >= 32 and ord(byte) < 128:
                    dest.write(byte)
                else:
                    dest.write(".")
            for i in xrange(bytesPerLine - len(srcLine)):
                dest.write(" ")
            dest.write("\n")
        return dest.getvalue()


    # Recursively build a list of (key,value) tuples that will be displayed
    # to represent a message. This handles traversing into substructures
    # like FlagUpdate.
    def buildKeys(object, prefix=""):
        keys = object.__dict__.keys()
        keys.sort()
        lst = []
        for key in keys:
            if key[0] != '_' and not key in ('eventLoop', 'socket', 'header'):
                value = object.__dict__[key]
                if isinstance(value, Protocol.Struct):
                    lst.extend(buildKeys(value, prefix + key + "."))
                else:
                    lst.append((prefix + key, value))
        return lst


    # Decide whether or not to show the message
    if options['hide'] and name in options['hide']:
        return
    if options['show'] and not name in options['show']:
        return

    # Always dump the name, but the contents can be disabled
    print "%s %s" % (direction, name)
    if not options['names']:
        for (key, value) in buildKeys(msg):
            if key == 'data':
                # Special decoding for 'data' members- do a hex dump
                value = hexDump(value)
            else:
                # Let python decode everything else
                value = repr(value)
                
            # Handle printing multiline values properly
            keyColumnWidth = 25
            lines = value.split("\n")
            print ("%%%ss: %%s" % keyColumnWidth) % (key, lines[0])
            for line in lines[1:]:
                if line:
                    print " " * (keyColumnWidth + 2) + line


# Set up events to forward messages between
# client and server, dumping them to stdout
def onClientMessage(msg):
    dumpMessage(msg, '<--')
    server.clients[client.id].write(msg)
    return 1
client.onAnyMessage.observe(onClientMessage)
def onServerMessage(msg):
    dumpMessage(msg, '-->')
    client.tcp.write(msg)
    return 1
server.onAnyMessage.observe(onServerMessage)


eventLoop.run()

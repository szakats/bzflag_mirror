""" BZFlag.Util

Small utilities that don't seem to fit anywhere else :)
"""
#
# Python BZFlag Protocol Package
# Copyright (C) 2003 Micah Dowty <micahjd@users.sourceforge.net>
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#


def getSubclassDict(module, baseClass, keyAttribute, cacheName='contentsDict'):
    """Return a dictionary of all the base classes of baseClass in module,
       keyed by keyAttribute. This dictionary is cached in the module itself
       under the attribute named cacheName.
       """
    if not hasattr(module, cacheName):
        d = {}
        for key in module.__dict__:
            try:
                value = module.__dict__[key]
                if issubclass(value, baseClass):
                    key = getattr(value, keyAttribute, None)
                    if key is not None:
                        d[key] = value
            except TypeError:
                pass
        setattr(module, cacheName, d)
    return getattr(module, cacheName)


def hexDump(src, dest, bytesPerLine=16, wordSize=2):
    """A pretty standard hex dumper routine.
       Dumps the stream 'src' to the stream 'dest'
       """
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


def messageDump(msg, f, showContents=True, keyColumnWidth=25):
    """Dump a message in human readable form to the stream 'f'."""
    from BZFlag import Protocol
    from StringIO import StringIO

    name = msg.__class__.__name__

    # If the message was received with Network.Socket.readMessage,
    # we can determine where it came from by its fromModule attribute
    direction = ''
    try:
        from BZFlag.Protocol import FromServer, ToServer
        if msg.fromModule == ToServer:
            direction = "-->"
        elif msg.fromModule == FromServer:
            direction = "<--"
    except AttributeError:
        pass

    f.write("%s %s\n" % (direction, name))
    if not showContents:
        return

    # Attributes for us to ignore. These are generally things we annotate the message
    # with later. Note that this isn't a list of all such annotations- most notably,
    # 'protocol' is omitted since it's very useful to have in message dumps.
    ignoreList = ('eventLoop', 'socket', 'header', 'fromModule', 'fromAddress', 'client')
    
    # Recursively build a list of (key,value) tuples that will be displayed
    # to represent a message. This handles traversing into substructures
    # like FlagUpdate.
    def buildKeys(object, prefix=""):
        keys = object.__dict__.keys()
        keys.sort()
        lst = []
        for key in keys:
            if key[0] != '_' and not key in ignoreList:
                value = object.__dict__[key]
                if isinstance(value, Protocol.Struct):
                    lst.extend(buildKeys(value, prefix + key + "."))
                else:
                    lst.append((prefix + key, value))
        return lst

    for (key, value) in buildKeys(msg):
        if key == 'data':
            # Special decoding for 'data' members- do a hex dump
            d = StringIO()
            hexDump(StringIO(value), d)
            value = ("%d bytes\n" % len(value)) + d.getvalue()
        else:
            # Let python decode everything else
            value = repr(value)
                
        # Handle printing multiline values properly
        lines = value.split("\n")
        print ("%%%ss: %%s" % keyColumnWidth) % (key, lines[0])
        for line in lines[1:]:
            if line:
                f.write(" " * (keyColumnWidth + 2) + line + "\n")


def proxy(server, client):
    """Set up a proxy between the server and client.
       Currently this only works correctly when there is one client
       connected to the server. It forwards all events (tcp and udp)
       from one to the other.
       """
    # Let the user know when our client connects, and force
    # our server to hand out the same client ID we were given.
    def onClientConnect():
        server.nextClientID = client.id
        server.clientIDIncrement = 0
    client.onConnect.observe(onClientConnect)

    # Set up events to forward messages between
    # client and server, dumping them to stdout
    def onClientMessage(msg):
        try:
            server.clientsByID[client.id].write(msg, msg.protocol)
        except KeyError:
            pass
    client.onUnhandledMessage.replace(onClientMessage)

    def onServerMessage(msg):
        getattr(client, msg.protocol).write(msg)
    server.onUnhandledMessage.replace(onServerMessage)


def autoFile(name, mode="r"):
    """Given a name which could be a local file, URI, or
       a file-like object, return a file-like object.
       """
    if type(name) == str:
        # If it doesn't look like a URL, don't bother loading urllib2
        if name.find("://") < 0:
            import os
            return open(os.path.expanduser(name), mode)
        else:
            import urllib2
            return urllib2.urlopen(name)
    else:
        return name


def dataFile(name):
    """Given the name of a data file, return its full path.
       Currently this has a hardcoded data path, it will later be configurable.
       """
    import os
    return os.path.join('data', name)

### The End ###

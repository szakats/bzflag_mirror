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
    
    # Recursively build a list of (key,value) tuples that will be displayed
    # to represent a message. This handles traversing into substructures
    # like FlagUpdate.
    def buildKeys(object, prefix=""):
        keys = object.__dict__.keys()
        keys.sort()
        lst = []
        for key in keys:
            if key[0] != '_' and not key in ('eventLoop', 'socket', 'header', 'fromModule'):
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

### The End ###

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

from StringIO import StringIO


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


### The End ###

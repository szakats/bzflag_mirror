""" BZFlag.Protocol.WorldObjects

Specification of behaviour and storage for all BZFlag world
objects. These are Structs that can be packed into binary
and back easily, plus they have methods for serializing to
ASCII map files, and methods for converting to 2D and 3D
representations.
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

from BZFlag.Protocol import *
from BZFlag.Protocol import Common
from BZFlag import Errors
import re, math


class BlockHeader(Struct):
    entries = [
        StructEntry(UInt16,   'id'),
        ]


class Block(Common.Message):
    """The world binary is made up of a header followed by a number of blocks.
       This is like a Message, but without the length field.
       """
    headerClass = BlockHeader
    messageId = None


class WorldObject(Block):
    """Building on the Struct and Message functionality, this base
       class adds methods for serializing to text.
       """
    # The section name used in text mode. None indicates that
    # the object should not be saved in text mode.
    textName = None
    
    def __init__(self, packed=None, **kw):
        self.unknownMapKeywords = {}
        Block.__init__(self, packed, **kw)
    
    def textRead(self, lines):
        """This accepts a list of text lines, with comments and
           leading/trailing whitespace already stripped out. Note that
           lines starting with '#!' should have the '#!' stripped out and
           be treated like normal lines, since that prefix is used
           to hide new map editor properties from older bzfs code.
           """
        for line in lines:
            tokens = re.split("\s+", line)
            keyword = tokens[0].lower()
            args = tokens[1:]
            self.unknownMapKeywords = {}
            if keyword == 'position':
                self.center = map(float, args)
            elif keyword == 'rotation':
                self.angle = float(args[0]) * math.pi / 180
            elif keyword == 'size':
                self.size = map(float, args)
            elif keyword == 'border':
                self.border = float(args[0])
            elif keyword == 'name':
                self.name = " ".join(args)
            elif keyword == 'from':
                self.fromSide = int(args[0])
            elif keyword == 'to':
                self.toSide = int(args[0])
            elif keyword == 'color':
                self.team = Common.TeamColor.dict[int(args[0])]
            elif keyword == 'end':
                break
            else:
                self.unknownMapKeywords[keyword] = args

    def textWrite(self, f):
        """Write a text representation of this object to
           the given file-like object.
           """
        if not self.textName:
            return
        f.write("%s\n" % self.textName)
        if hasattr(self, 'name'):
            f.write("#!name %s\n" % self.name)
        if hasattr(self, 'center'):
            f.write("position %s\n" % " ".join(map(str, self.center)))
        if hasattr(self, 'size'):
            f.write("size %s\n" % " ".join(map(str, self.size)))
        if hasattr(self, 'border'):
            f.write("border %s\n" % self.border)
        if hasattr(self, 'angle'):
            f.write("rotation %s\n" % (self.angle * 180 / math.pi))
        if hasattr(self, 'fromSide'):
            f.write("from %s\n" % self.fromSide)
        if hasattr(self, 'toSide'):
            f.write("to %s\n" % self.toSide)
        if hasattr(self, 'team'):
            f.write("color %s\n" % Common.TeamColor.reverseDict(self.team))
        f.write("end\n\n")
        
    def toPolygon(self):
        """Return a 2D polygon representation of this object, as a tuple of tuples."""
        # Create four vertices from our extents
        poly = ((-self.size[0],
                 -self.size[1]),
                ( self.size[0],
                 -self.size[1]),
                ( self.size[0],
                  self.size[1]),
                (-self.size[0],
                  self.size[1]))

        # Rotate the polygon by our angle
        cos = math.cos(-self.angle)
        sin = math.sin(-self.angle)
        def rotate(point):
            return ( cos*point[0] + sin*point[1],
                    -sin*point[0] + cos*point[1])
        poly = map(rotate, poly)

        # Translate it to our center location
        def translate(point):
            return (point[0] + self.center[0],
                    point[1] + self.center[1])
        return map(translate, poly)

        
class Style(Block):
    messageId = 0x7374
    entries = [
        # This first field is the size, but since this is the only
        # block that has it, we'll just treat it as a magical constant.
        ConstStructEntry(UInt16, 28,  Errors.ProtocolError("Bad size in world style block")),
        StructEntry(Float,            'worldSize',            800.0),
        StructEntry(Common.GameStyle, 'gameStyle',            []),
        StructEntry(UInt16,           'players',              20),
        StructEntry(UInt16,           'shots',                1),
        StructEntry(UInt16,           'flags',                0),
        StructEntry(Float,            'linearAcceleration',   0.0),
        StructEntry(Float,            'angularAcceleration',  0.0),
        StructEntry(UInt16,           'shakeTime',            16138),
        StructEntry(UInt16,           'shakeWins',            22155),
        StructEntry(UInt32,           'serverTime',           0),
        ]

class TeamBase(WorldObject):
    textName = 'base'
    messageId = 0x6261
    entries = [
        StructEntry(Common.TeamColor, 'team'),
        StructEntry(Common.Vector3,   'center', [0,0,0]),
        StructEntry(Float,            'angle',  0),
        StructEntry(Common.Vector2,   'size',   [1,1,1]),
        StructEntry(Common.Vector3,   'safety', [0,0,0]),
        ]

class Wall(WorldObject):
    messageId = 0x776C
    entries = [
        StructEntry(Common.Vector3,   'center', [0,0,0]),
        StructEntry(Float,            'angle',  0),
        StructEntry(Common.Vector2,   'size',   [400.0, 6.15]),
        ]

class Box(WorldObject):
    textName = 'box'
    messageId = 0x6278
    entries = [
        StructEntry(Common.Vector3,   'center',       [0,0,0]),
        StructEntry(Float,            'angle',        0),
        StructEntry(Common.Vector3,   'size',         [1,1,1]),
        StructEntry(UInt8,            'driveThrough', 0),
        StructEntry(UInt8,            'shootThrough', 0),
        ]

class Pyramid(WorldObject):
    textName = 'pyramid'
    messageId = 0x7079
    entries = [
        StructEntry(Common.Vector3,   'center',       [0,0,0]),
        StructEntry(Float,            'angle',        0),
        StructEntry(Common.Vector3,   'size',         [1,1,1]),
        StructEntry(UInt8,            'driveThrough', 0),
        StructEntry(UInt8,            'shootThrough', 0),
        StructEntry(UInt8,            'flipZ',        0),
        ]

class Teleporter(WorldObject):
    textName = 'teleporter'
    messageId = 0x7465
    entries = [
        StructEntry(Common.Vector3,   'center',       [0,0,0]),
        StructEntry(Float,            'angle',        0),
        StructEntry(Common.Vector3,   'size',         [1,1,1]),
        StructEntry(UInt8,            'driveThrough', 0),
        StructEntry(UInt8,            'shootThrough', 0),
        StructEntry(Float,            'border',       1.0),
        ]

class TeleporterLink(WorldObject):
    textName = 'link'
    messageId = 0x6C6E
    entries = [
        StructEntry(UInt16, 'fromSide'),
        StructEntry(UInt16, 'toSide'),
        ]

class EndOfData(Block):
    messageId = 0x6564

### The End ###

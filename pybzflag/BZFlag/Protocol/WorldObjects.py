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
        StructEntry(UInt16, 'id'),
        ]


class Block(Common.Message):
    """The world binary is made up of a header followed by a number of blocks.
       This is like a Message, but without the length field.
       """
    headerClass = BlockHeader
    messageId = None


class GLDrawable:
    def __init__(self):
    	import OpenGL.GL
        self.list = OpenGL.GL.glGenLists(1)
	self.texture = None

    def __del__(self):
        import OpenGL.GL
	OpenGL.GL.glDeleteLists(self.list, 1)

    def draw(self):
        pass

    def drawWithName(self, name):
        pass


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

    def toUntransformedPolygon(self):
        # Create four vertices from our extents
        return ((-self.size[0],
                 -self.size[1]),
                ( self.size[0],
                  -self.size[1]),
                ( self.size[0],
                  self.size[1]),
                (-self.size[0],
                  self.size[1]))

    def toPolygon(self):
        """Return a 2D polygon representation of this object, as a tuple of tuples."""
        poly = self.toUntransformedPolygon()

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

    def getGLDrawables(self):
        return [GLDrawable()]


class Style(WorldObject):
    messageId = 0x7374
    entries = [
        # This first field is the size, but since this is the only
        # block that has it, we'll just treat it as a magical constant.
        ConstStructEntry(UInt16, 30,  Errors.ProtocolError("Bad size in world style block")),
        ConstStructEntry(UInt16, 1,   Errors.ProtocolError("Bad version in world size block")),
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

    class GroundDrawable(GLDrawable):
        def __init__(self, size):
	    import OpenGL.GL
	    import BZFlag.UI.Texture
	    GLDrawable.__init__(self)
	    self.size = size / 2
	    self.texture = BZFlag.UI.Texture.Texture('data/ground.png')
	    OpenGL.GL.glNewList(self.list, OpenGL.GL.GL_COMPILE)
	    OpenGL.GL.glPushMatrix()
	    OpenGL.GL.glDisable(OpenGL.GL.GL_CULL_FACE)
	    OpenGL.GL.glColor3f(1.0, 1.0, 1.0)
	    OpenGL.GL.glBegin(OpenGL.GL.GL_QUADS)
	    OpenGL.GL.glTexCoord2f(0.0, 0.0)
	    OpenGL.GL.glVertex3f(self.size, -self.size, 0)
	    OpenGL.GL.glTexCoord2f(1.0, 0.0)
	    OpenGL.GL.glVertex3f(-self.size, -self.size, 0)
	    OpenGL.GL.glTexCoord2f(1.0, 1.0)
	    OpenGL.GL.glVertex3f(-self.size, self.size, 0)
	    OpenGL.GL.glTexCoord2f(0.0, 1.0)
	    OpenGL.GL.glVertex3f(self.size, self.size, 0)
	    OpenGL.GL.glEnd()
	    OpenGL.GL.glEnable(OpenGL.GL.GL_CULL_FACE)
	    OpenGL.GL.glPopMatrix()
	    OpenGL.GL.glEndList()

	def draw(self):
	    import OpenGL.GL
	    OpenGL.GL.glCallList(self.list)

    def getGLDrawables(self):
        return [self.GroundDrawable(self.worldSize)]


ObjectOptions = Bitfield(UInt8, {
    'driveThrough':  0x01,
    'shootThrough':  0x02,
    'flipZ':         0x04,
    })


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
        StructEntry(Common.VectorYZ,  'size',   [0,1,1]),
        ]

    class WallDrawable(GLDrawable):
        def __init__(self, center, angle, size):
	    import OpenGL.GL
	    import BZFlag.UI.Texture
	    GLDrawable.__init__(self)
	    self.center = center
	    self.angle = angle * 180 / 3.1415926
	    self.size = size
	    self.texture = BZFlag.UI.Texture.Texture('data/wall.png')
	    OpenGL.GL.glNewList(self.list, OpenGL.GL.GL_COMPILE)
	    OpenGL.GL.glPushMatrix()
	    OpenGL.GL.glTranslatef(*self.center)
	    OpenGL.GL.glRotatef(self.angle, 0.0, 0.0, 1.0)
	    OpenGL.GL.glDisable(OpenGL.GL.GL_CULL_FACE)
	    OpenGL.GL.glColor3f(1.0, 1.0, 1.0)
	    OpenGL.GL.glBegin(OpenGL.GL.GL_TRIANGLE_STRIP)
	    OpenGL.GL.glTexCoord2f(0, 0)
	    OpenGL.GL.glVertex3f(0, -self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(0, 1)
	    OpenGL.GL.glVertex3f(0, -self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f((self.size[1] * 2) / self.size[2], 0)
	    OpenGL.GL.glVertex3f(0, self.size[1], 0)
	    OpenGL.GL.glTexCoord2f((self.size[1] * 2) / self.size[2], 1)
	    OpenGL.GL.glVertex3f(0, self.size[1], self.size[2])
	    OpenGL.GL.glEnd()
	    OpenGL.GL.glEnable(OpenGL.GL.GL_CULL_FACE)
	    OpenGL.GL.glPopMatrix()
	    OpenGL.GL.glEndList()

	def draw(self):
	    import OpenGL.GL
	    OpenGL.GL.glCallList(self.list)

    def getGLDrawables(self):
        return [self.WallDrawable(self.center, self.angle, self.size)]


class Box(WorldObject):
    textName = 'box'
    messageId = 0x6278
    entries = [
        StructEntry(Common.Vector3,   'center',       [0,0,0]),
        StructEntry(Float,            'angle',        0),
        StructEntry(Common.Vector3,   'size',         [1,1,1]),
        StructEntry(ObjectOptions,    'options',      []),
        ]

    class SidesDrawable(GLDrawable):
        def __init__(self, center, angle, size):
	    import OpenGL.GL
	    import BZFlag.UI.Texture
	    GLDrawable.__init__(self)
	    self.center = center
	    self.angle = angle * 180 / 3.1415926
	    self.size = size
	    self.texture = BZFlag.UI.Texture.Texture('data/boxwall.png')
	    OpenGL.GL.glNewList(self.list, OpenGL.GL.GL_COMPILE)
	    OpenGL.GL.glPushMatrix()
	    OpenGL.GL.glTranslatef(*self.center)
	    OpenGL.GL.glRotatef(self.angle, 0.0, 0.0, 1.0)
	    OpenGL.GL.glBegin(OpenGL.GL.GL_QUADS)
	    # Y+ side
	    OpenGL.GL.glNormal3f(0, 1, 0)
	    OpenGL.GL.glTexCoord2f(0, 0)
	    OpenGL.GL.glVertex3f(-self.size[0], self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(0, self.size[2] / 4)
	    OpenGL.GL.glVertex3f(-self.size[0], self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, self.size[2] / 4)
	    OpenGL.GL.glVertex3f(self.size[0], self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, 0)
	    OpenGL.GL.glVertex3f(self.size[0], self.size[1], 0)
	    # Y- side
	    OpenGL.GL.glNormal3f(0, -1, 0)
	    OpenGL.GL.glTexCoord2f(0, 0)
	    OpenGL.GL.glVertex3f(self.size[0], -self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(0, self.size[2] / 4)
	    OpenGL.GL.glVertex3f(self.size[0], -self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, self.size[2] / 4)
	    OpenGL.GL.glVertex3f(-self.size[0], -self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, 0)
	    OpenGL.GL.glVertex3f(-self.size[0], -self.size[1], 0)
	    # X+ side
	    OpenGL.GL.glNormal3f(1, 0, 0)
	    OpenGL.GL.glTexCoord2f(0, 0)
	    OpenGL.GL.glVertex3f(self.size[0], self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(0, self.size[2] / 4)
	    OpenGL.GL.glVertex3f(self.size[0], self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[1] / 2, self.size[2] / 4)
	    OpenGL.GL.glVertex3f(self.size[0], -self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[1] / 2, 0)
	    OpenGL.GL.glVertex3f(self.size[0], -self.size[1], 0)
	    # X- side
	    OpenGL.GL.glNormal3f(-1, 0, 0)
	    OpenGL.GL.glTexCoord2f(0, 0)
	    OpenGL.GL.glVertex3f(-self.size[0], -self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(0, self.size[2] / 4)
	    OpenGL.GL.glVertex3f(-self.size[0], -self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[1] / 2, self.size[2] / 4)
	    OpenGL.GL.glVertex3f(-self.size[0], self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[1] / 2, 0)
	    OpenGL.GL.glVertex3f(-self.size[0], self.size[1], 0)
	    OpenGL.GL.glEnd()
	    OpenGL.GL.glPopMatrix()
	    OpenGL.GL.glEndList()

	def draw(self):
	    import OpenGL.GL
	    OpenGL.GL.glCallList(self.list)

    class TopsDrawable(GLDrawable):
        def __init__(self, center, angle, size):
	    import OpenGL.GL
	    import BZFlag.UI.Texture
	    GLDrawable.__init__(self)
	    self.center = center
	    self.angle = angle * 180 / 3.1415926
	    self.size = size
	    self.texture = BZFlag.UI.Texture.Texture('data/boxtops.png')
	    OpenGL.GL.glNewList(self.list, OpenGL.GL.GL_COMPILE)
	    OpenGL.GL.glPushMatrix()
	    OpenGL.GL.glTranslatef(*self.center)
	    OpenGL.GL.glRotatef(self.angle, 0.0, 0.0, 1.0)
	    OpenGL.GL.glBegin(OpenGL.GL.GL_QUADS)
	    # Z+ side
	    OpenGL.GL.glNormal3f(0, 0, 1)
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, 0)
	    OpenGL.GL.glVertex3f(self.size[0], -self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
	    OpenGL.GL.glVertex3f(self.size[0], self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(0, self.size[1] / 2)
	    OpenGL.GL.glVertex3f(-self.size[0], self.size[1], self.size[2])
	    OpenGL.GL.glTexCoord2f(0, 0)
	    OpenGL.GL.glVertex3f(-self.size[0], -self.size[1], self.size[2])
	    # Z- side
	    OpenGL.GL.glNormal3f(0, 0, -1)
	    OpenGL.GL.glTexCoord2f(0, 0)
	    OpenGL.GL.glVertex3f(-self.size[0], -self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(0, self.size[1] / 2)
	    OpenGL.GL.glVertex3f(-self.size[0], self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
	    OpenGL.GL.glVertex3f(self.size[0], self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, 0)
	    OpenGL.GL.glVertex3f(self.size[0], -self.size[1], 0)
	    OpenGL.GL.glEnd()
	    OpenGL.GL.glPopMatrix()
	    OpenGL.GL.glEndList()

	def draw(self):
	    import OpenGL.GL
	    OpenGL.GL.glCallList(self.list)

    def getGLDrawables(self):
        return [
	    self.SidesDrawable(self.center, self.angle, self.size),
	    self.TopsDrawable(self.center, self.angle, self.size),
	    ]


class Pyramid(WorldObject):
    textName = 'pyramid'
    messageId = 0x7079
    entries = [
        StructEntry(Common.Vector3,   'center',       [0,0,0]),
        StructEntry(Float,            'angle',        0),
        StructEntry(Common.Vector3,   'size',         [1,1,1]),
        StructEntry(ObjectOptions,    'options',      []),
        ]

    class PyramidDrawable(GLDrawable):
        def __init__(self, center, angle, size, flip):
	    # FIXME - respect flipz
	    import OpenGL.GL
	    import BZFlag.UI.Texture
	    GLDrawable.__init__(self)
	    self.center = center
	    self.angle = angle * 180 / 3.1415926
	    self.size = size;
	    self.flip = flip;
	    OpenGL.GL.glNewList(self.list, OpenGL.GL.GL_COMPILE)
	    OpenGL.GL.glPushMatrix()
	    OpenGL.GL.glTranslatef(*self.center)
	    OpenGL.GL.glRotatef(self.angle, 0.0, 0.0, 1.0)
	    OpenGL.GL.glBegin(OpenGL.GL.GL_QUADS)
	    # Z- side
	    OpenGL.GL.glNormal3f(0, 0, -1)
	    OpenGL.GL.glTexCoord2f(0, 0)
	    OpenGL.GL.glVertex3f(-self.size[0], -self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(0, self.size[1] / 2)
	    OpenGL.GL.glVertex3f(-self.size[0], self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
	    OpenGL.GL.glVertex3f(self.size[0], self.size[1], 0)
	    OpenGL.GL.glTexCoord2f(self.size[0] / 2, 0)
	    OpenGL.GL.glVertex3f(self.size[0], -self.size[1], 0)
	    OpenGL.GL.glEnd()
	    OpenGL.GL.glBegin(OpenGL.GL.GL_TRIANGLES)
	    # FIXME - implement vector cross product to do this correctly
	    # X+ side
	    # X- side
	    # Y+ side
	    # Y- side
	    OpenGL.GL.glEnd()
	    OpenGL.GL.glPopMatrix()
	    OpenGL.GL.glEndList()

	def draw(self):
	    import OpenGL.GL
	    OpenGL.GL.glCallList(self.list)

    def getGLDrawables(self):
        return [self.PyramidDrawable(self.center, self.angle, self.size, False)]


class Teleporter(WorldObject):
    textName = 'teleporter'
    messageId = 0x7465
    entries = [
        StructEntry(Common.Vector3,   'center',       [0,0,0]),
        StructEntry(Float,            'angle',        0),
        StructEntry(Common.Vector3,   'size',         [1,1,1]),
        StructEntry(ObjectOptions,    'options',      []),
        StructEntry(Float,            'border',       1.0),
        ]

    class FieldDrawable(GLDrawable):
        def __init__(self, center, angle, size):
	    import OpenGL.GL
	    GLDrawable.__init__(self)
	    self.center = center
	    self.angle = angle * 180 / 3.1415926
	    self.size = size
	    OpenGL.GL.glNewList(self.list, OpenGL.GL.GL_COMPILE)
	    OpenGL.GL.glPushMatrix()
	    OpenGL.GL.glTranslatef(*self.center)
	    OpenGL.GL.glRotatef(self.angle, 0.0, 0.0, 1.0)
	    OpenGL.GL.glColor3f(0.1, 0.1, 0.1)
	    OpenGL.GL.glBegin(OpenGL.GL.GL_QUADS)
	    # X+ side
	    OpenGL.GL.glNormal3f(1, 0, 0)
	    OpenGL.GL.glVertex3f(self.size[0] / 2, self.size[1], 0)
	    OpenGL.GL.glVertex3f(self.size[0] / 2, self.size[1], self.size[2])
	    OpenGL.GL.glVertex3f(self.size[0] / 2, -self.size[1], self.size[2])
	    OpenGL.GL.glVertex3f(self.size[0] / 2, -self.size[1], 0)
	    # X- side
	    OpenGL.GL.glNormal3f(-1, 0, 0)
	    OpenGL.GL.glVertex3f(-self.size[0] / 2, -self.size[1], 0)
	    OpenGL.GL.glVertex3f(-self.size[0] / 2, -self.size[1], self.size[2])
	    OpenGL.GL.glVertex3f(-self.size[0] / 2, self.size[1], self.size[2])
	    OpenGL.GL.glVertex3f(-self.size[0] / 2, self.size[1], 0)
	    OpenGL.GL.glEnd()
	    OpenGL.GL.glColor3f(1.0, 1.0, 1.0)
	    OpenGL.GL.glPopMatrix()
	    OpenGL.GL.glEndList()

	def draw(self):
	    import OpenGL.GL
	    OpenGL.GL.glCallList(self.list)

    def getGLDrawables(self):
        return [self.FieldDrawable(self.center, self.angle, self.size)]


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

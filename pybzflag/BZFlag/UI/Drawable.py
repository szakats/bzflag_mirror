""" BZFlag.UI.Drawable

OpenGL drawing definitions for various world objects
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

from OpenGL.GL import *
from BZFlag.UI.Texture import Texture
from BZFlag import Util


def cross(a, b):
    """3-Vector cross product"""
    return (a[2] * b[1] - a[1] * b[2], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])


class GLDrawable:
    """Abstract base class for an object that can be drawn to an OpenGL context.
       This is the basic unit used by the ThreeDView to sort objects by texture,
       so it is required that the drawable have at most one texture.
       The blended flag is used to put objects that need blending into a
       second rendering pass.
       """
    textureName = None

    def __init__(self):
        if self.textureName:
            self.texture = Texture(Util.dataFile(self.textureName))
        else:
            self.texture = None
        self.blended = False

    def draw(self):
        pass

    def drawWithName(self, name):
        """Draw the object with the given name. This is used for OpenGL object picking"""
        pass


class DisplayList(GLDrawable):
    """A drawable that stores itself to a display list before rendering"""
    def __init__(self, *args, **kw):
        GLDrawable.__init__(self)
        self.list = glGenLists(1)
        self.init(*args, **kw)

    def init(self, *args, **kw):
        """Called on init after setting up the display list. By default this
           builds the display lists, but this hook lets subclasses override that.
           """
        self.buildList(*args, **kw)

    def __del__(self):
        glDeleteLists(self.list, 1)

    def buildList(self, *args, **kw):
        """Rebuild this object's display list. Arguments depend on the
           subclass' drawToList mehtod's arguments.
           """
        glNewList(self.list, GL_COMPILE)
        self.drawToList(*args, **kw)
        glEndList()

    def drawToList(self):
        """Stub where subclasses will do their drawing"""
        pass

    def draw(self):
        glCallList(self.list)


class Ground(DisplayList):
    textureName = 'ground.png'
    def drawToList(self, size):
        self.size = size / 2
        glPushMatrix()
        glDisable(GL_CULL_FACE)
        glBegin(GL_QUADS)
        texRepeats = 60
        glNormal3f(0, 0, 1);
        glTexCoord2f(0.0, 0.0)
        glVertex3f(self.size, -self.size, 0)
        glTexCoord2f(texRepeats, 0.0)
        glVertex3f(-self.size, -self.size, 0)
        glTexCoord2f(texRepeats, texRepeats)
        glVertex3f(-self.size, self.size, 0)
        glTexCoord2f(0.0, texRepeats)
        glVertex3f(self.size, self.size, 0)
        glEnd()
        glEnable(GL_CULL_FACE)
        glPopMatrix()


class BaseTops(DisplayList):
    def drawToList(self, team, center, angle, size):
        self.team = team
        self.center = center
        self.angle = angle
        self.size = size
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        if self.team == 'red':
            glColor3f(1.0, 0.0, 0.0)
        if self.team == 'green':
            glColor3f(0.0, 1.0, 0.0)
        if self.team == 'blue':
            glColor3f(0.0, 0.0, 1.0)
        if self.team == 'purple':
            glColor3f(1.0, 0.0, 1.0)
        glBegin(GL_QUADS)
        # Z+
        glNormal3f(0, 0, 1);
        glVertex3f(self.size[0], -self.size[1], self.size[2]);
        glVertex3f(self.size[0], self.size[1], self.size[2]);
        glVertex3f(-self.size[0], self.size[1], self.size[2]);
        glVertex3f(-self.size[0], -self.size[1], self.size[2]);
        # Z-
        glNormal3f(0, 0, -1);
        glVertex3f(-self.size[0], -self.size[1], 0);
        glVertex3f(-self.size[0], self.size[1], 0);
        glVertex3f(self.size[0], self.size[1], 0);
        glVertex3f(self.size[0], -self.size[1], 0);
        glEnd()
        glColor3f(1.0, 1.0, 1.0)
        glPopMatrix()


class Wall(DisplayList):
    textureName = 'wall.png'
    def drawToList(self, center, angle, size):
        self.center = center
        self.angle = angle
        self.size = size
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glDisable(GL_CULL_FACE)
        glBegin(GL_TRIANGLE_STRIP)
        glTexCoord2f(0, 0)
        glVertex3f(0, -self.size[1], 0)
        glTexCoord2f(0, 1)
        glVertex3f(0, -self.size[1], self.size[2])
        glTexCoord2f((self.size[1] * 2) / self.size[2], 0)
        glVertex3f(0, self.size[1], 0)
        glTexCoord2f((self.size[1] * 2) / self.size[2], 1)
        glVertex3f(0, self.size[1], self.size[2])
        glEnd()
        glEnable(GL_CULL_FACE)
        glPopMatrix()


class BoxSides(DisplayList):
    textureName = 'boxwall.png'
    def drawToList(self, center, angle, size):
        self.center = center
        self.angle = angle
        self.size = size
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glBegin(GL_QUADS)
        # Y+ side
        glNormal3f(0, 1, 0)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], self.size[1], 0)
        glTexCoord2f(0, self.size[2] / 4)
        glVertex3f(-self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.size[0] / 2, self.size[2] / 4)
        glVertex3f(self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(self.size[0], self.size[1], 0)
        # Y- side
        glNormal3f(0, -1, 0)
        glTexCoord2f(0, 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        glTexCoord2f(0, self.size[2] / 4)
        glVertex3f(self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.size[0] / 2, self.size[2] / 4)
        glVertex3f(-self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        # X+ side
        glNormal3f(1, 0, 0)
        glTexCoord2f(0, 0)
        glVertex3f(self.size[0], self.size[1], 0)
        glTexCoord2f(0, self.size[2] / 4)
        glVertex3f(self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.size[1] / 2, self.size[2] / 4)
        glVertex3f(self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.size[1] / 2, 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        # X- side
        glNormal3f(-1, 0, 0)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        glTexCoord2f(0, self.size[2] / 4)
        glVertex3f(-self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.size[1] / 2, self.size[2] / 4)
        glVertex3f(-self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.size[1] / 2, 0)
        glVertex3f(-self.size[0], self.size[1], 0)
        glEnd()
        glPopMatrix()


class BoxTops(DisplayList):
    textureName = 'boxtops.png'
    def drawToList(self, center, angle, size):
        self.center = center
        self.angle = angle
        self.size = size
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glBegin(GL_QUADS)
        # Z+ side
        glNormal3f(0, 0, 1)
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
        glVertex3f(self.size[0], self.size[1], self.size[2])
        glTexCoord2f(0, self.size[1] / 2)
        glVertex3f(-self.size[0], self.size[1], self.size[2])
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], -self.size[1], self.size[2])
        # Z- side
        glNormal3f(0, 0, -1)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        glTexCoord2f(0, self.size[1] / 2)
        glVertex3f(-self.size[0], self.size[1], 0)
        glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
        glVertex3f(self.size[0], self.size[1], 0)
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        glEnd()
        glPopMatrix()


class Pyramid(DisplayList):
    textureName = 'pyrwall.png'
    def drawToList(self, center, angle, size, flip):
        # FIXME - respect flipz
        self.center = center
        self.angle = angle
        self.size = size;
        self.flip = flip;
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glBegin(GL_QUADS)
        # Z- side
        glNormal3f(0, 0, -1)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        glTexCoord2f(0, self.size[1] / 2)
        glVertex3f(-self.size[0], self.size[1], 0)
        glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
        glVertex3f(self.size[0], self.size[1], 0)
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        glEnd()
        glBegin(GL_TRIANGLES)
        # X+ side
        norm = cross((self.size[0], -self.size[1], self.size[2]), (self.size[0] * 2, 0, 0))
        glNormal3f(*norm)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], self.size[1], 0)
        glTexCoord2f(self.size[0] / 4, self.size[2] / 4)
        glVertex3f(0, 0, self.size[2])
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(self.size[0], self.size[1], 0)
        # Y+ side
        norm = cross((self.size[0], -self.size[1], self.size[2]), (0, self.size[1] * 2, 0))
        glNormal3f(*norm)
        glTexCoord2f(0, 0)
        glVertex3f(self.size[0], self.size[1], 0)
        glTexCoord2f(self.size[1] / 4, self.size[2] / 4)
        glVertex3f(0, 0, self.size[2])
        glTexCoord2f(self.size[1] / 2, 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        # X- side
        norm = cross((-self.size[0], self.size[1], self.size[2]), (-self.size[0] * 2, 0, 0))
        glNormal3f(*norm)
        glTexCoord2f(0, 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        glTexCoord2f(self.size[0] / 4, self.size[2] / 4)
        glVertex3f(0, 0, self.size[2])
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        # Y- side
        norm = cross((-self.size[0], self.size[1], self.size[2]), (0, -self.size[1] * 2, 0))
        glNormal3f(*norm)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        glTexCoord2f(self.size[1] / 4, self.size[2] / 4)
        glVertex3f(0, 0, self.size[2])
        glTexCoord2f(self.size[1] / 2, 0)
        glVertex3f(-self.size[0], self.size[1], 0)
        glEnd()
        glPopMatrix()


class TeleporterField(DisplayList):
    def drawToList(self, center, angle, size):
        self.center = center
        self.angle = angle
        self.size = size
        self.blended = True
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glColor4f(0.0, 0.0, 0.0, 0.3)
        glDepthMask(0)
        glBegin(GL_QUADS)
        # X+ side
        glNormal3f(1, 0, 0)
        glVertex3f(self.size[0] / 2, self.size[1], 0)
        glVertex3f(self.size[0] / 2, self.size[1], self.size[2])
        glVertex3f(self.size[0] / 2, -self.size[1], self.size[2])
        glVertex3f(self.size[0] / 2, -self.size[1], 0)
        # X- side
        glNormal3f(-1, 0, 0)
        glVertex3f(-self.size[0] / 2, -self.size[1], 0)
        glVertex3f(-self.size[0] / 2, -self.size[1], self.size[2])
        glVertex3f(-self.size[0] / 2, self.size[1], self.size[2])
        glVertex3f(-self.size[0] / 2, self.size[1], 0)
        glEnd()
        glDepthMask(1)
        glColor3f(1.0, 1.0, 1.0)
        glPopMatrix()


class TeleporterBorder(DisplayList):
    textureName = 'caution.png'
    def drawToList(self, center, angle, size, border):
        self.center = center
        self.angle = angle
        self.size = size
        self.border = border
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glBegin(GL_TRIANGLE_STRIP)
        glNormal3f(-1, 0, 0)
        glTexCoord2f(0, 0)
        glVertex3f(-self.border / 2, self.size[1] + self.border, 0)
        glTexCoord2f(self.border, 0)
        glVertex3f(-self.border / 2, self.size[1], 0)
        glTexCoord2f(0, self.size[2] + self.border)
        glVertex3f(-self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
        glTexCoord2f(self.border, self.size[2])
        glVertex3f(-self.border / 2, self.size[1], self.size[2])
        glTexCoord2f(2 * self.border + 2 * self.size[1], self.size[2] + self.border)
        glVertex3f(-self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
        glTexCoord2f(self.border + 2 * self.size[1], self.size[2])
        glVertex3f(-self.border / 2, -self.size[1], self.size[2])
        glTexCoord2f(2 * self.border + 2 * self.size[1], 0)
        glVertex3f(-self.border / 2, -self.size[1] - self.border, 0)
        glTexCoord2f(self.border + 2 * self.size[1], 0)
        glVertex3f(-self.border / 2, -self.size[1], 0)
        glEnd()
        glFrontFace(GL_CW)
        glBegin(GL_TRIANGLE_STRIP)
        glNormal3f(1, 0, 0)
        glTexCoord2f(0, 0)
        glVertex3f(self.border / 2, self.size[1] + self.border, 0)
        glTexCoord2f(self.border, 0)
        glVertex3f(self.border / 2, self.size[1], 0)
        glTexCoord2f(0, self.size[2] + border)
        glVertex3f(self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
        glTexCoord2f(self.border, self.size[2])
        glVertex3f(self.border / 2, self.size[1], self.size[2])
        glTexCoord2f(2 * self.border + 2 * self.size[1], self.size[2] + self.border)
        glVertex3f(self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
        glTexCoord2f(self.border + 2 * self.size[1], self.size[2])
        glVertex3f(self.border / 2, -self.size[1], self.size[2])
        glTexCoord2f(2 * self.border + 2 * self.size[1], 0)
        glVertex3f(self.border / 2, -self.size[1] - self.border, 0)
        glTexCoord2f(self.border + 2 * self.size[1], 0)
        glVertex3f(self.border / 2, -self.size[1], 0)
        glEnd()
        glFrontFace(GL_CCW)
        glBegin(GL_QUADS)
        # top
        glNormal3f(0.4, 0, 1)
        glTexCoord2f(0.4, 0.0001)
        glVertex3f(self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
        glTexCoord2f(2 * self.border + 2 * self.size[1] + 0.4, 0.0001)
        glVertex3f(self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
        glTexCoord2f(2 * self.border + 2 * self.size[1] + 0.4, 0)
        glVertex3f(-self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
        glTexCoord2f(0.4, 0)
        glVertex3f(-self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
        # underside of top
        glNormal3f(0, 0, -1)
        glTexCoord2f(0.4, 0)
        glVertex3f(-self.border / 2, -self.size[1] - self.border, self.size[2])
        glTexCoord2f(2 * self.border + 2 * self.size[1] + 0.4, 0)
        glVertex3f(-self.border / 2, self.size[1] + self.border, self.size[2])
        glTexCoord2f(2 * self.border + 2 * self.size[1] + 0.4, 0.0001)
        glVertex3f(self.border / 2, self.size[1] + self.border, self.size[2])
        glTexCoord2f(0.4, 0.0001)
        glVertex3f(self.border / 2, -self.size[1] - self.border, self.size[2])
        # Y+ outside
        glNormal3f(0, 1, 0)
        glTexCoord2f(self.border, 0)
        glVertex3f(-self.border / 2, self.size[1] + border, 0)
        glTexCoord2f(self.border, self.size[2] + border)
        glVertex3f(-self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
        glTexCoord2f(0, self.size[2] + self.border)
        glVertex3f(self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
        glTexCoord2f(0, 0)
        glVertex3f(self.border / 2, self.size[1] + self.border, 0)
        # Y+ inside
        glNormal3f(0, -1, 0)
        glTexCoord2f(0, 0)
        glVertex3f(self.border / 2, self.size[1], 0)
        glTexCoord2f(0, self.size[2])
        glVertex3f(self.border / 2, self.size[1], self.size[2])
        glTexCoord2f(self.border, self.size[2])
        glVertex3f(-self.border / 2, self.size[1], self.size[2])
        glTexCoord2f(self.border, 0)
        glVertex3f(-self.border / 2, self.size[1], 0);
        # Y- outside
        glNormal3f(0, 1, 0)
        glTexCoord2f(0, 0)
        glVertex3f(self.border / 2, -self.size[1] - self.border, 0)
        glTexCoord2f(0, self.size[2] + self.border)
        glVertex3f(self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
        glTexCoord2f(self.border, self.size[2] + self.border)
        glVertex3f(-self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
        glTexCoord2f(self.border, 0)
        glVertex3f(-self.border / 2, -self.size[1] - self.border, 0)
        # Y- inside
        glNormal3f(0, -1, 0)
        glTexCoord2f(self.border, 0)
        glVertex3f(-self.border / 2, -self.size[1], 0)
        glTexCoord2f(self.border, self.size[2])
        glVertex3f(-self.border / 2, -self.size[1], self.size[2])
        glTexCoord2f(0, self.size[2])
        glVertex3f(self.border / 2, -self.size[1], self.size[2])
        glTexCoord2f(0, 0)
        glVertex3f(self.border / 2, -self.size[1], 0)
        # Y+ leg bottom
        glNormal3f(0, 0, -1)
        glTexCoord2f(self.border, self.border)
        glVertex3f(self.border / 2, self.size[1] + self.border, 0)
        glTexCoord2f(0, self.border)
        glVertex3f(self.border / 2, self.size[1], 0)
        glTexCoord2f(0, 0)
        glVertex3f(-self.border / 2, self.size[1], 0)
        glTexCoord2f(self.border, 0)
        glVertex3f(-self.border / 2, self.size[1] + self.border, 0)
        # Y- leg bottom
        glNormal3f(0, 0, -1);
        glTexCoord2f(self.border, self.border);
        glVertex3f(-self.border / 2, -self.size[1] - self.border, 0);
        glTexCoord2f(self.border, 0);
        glVertex3f(-self.border / 2, -self.size[1], 0);
        glTexCoord2f(0, 0);
        glVertex3f(self.border / 2, -self.size[1], 0);
        glTexCoord2f(0, self.border);
        glVertex3f(self.border / 2, -self.size[1] - self.border, 0);
        glEnd()
        glPopMatrix()

### The End ###

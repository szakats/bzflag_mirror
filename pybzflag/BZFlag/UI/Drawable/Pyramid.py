""" BZFlag.UI.Drawable.Pyramid

A class for drawing the pyramids in the world
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
from DisplayList import *
from OpenGL.GL import *


def cross(a, b):
    """3-Vector cross product"""
    return (a[2] * b[1] - a[1] * b[2], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])


class Pyramid(DisplayList):
    textureName = 'pyrwall.png'
    def set(self, center, angle, size, flip):
        self.center = center
        self.angle = angle
        self.size = size
        self.flip = flip

    def drawToList(self):
        z = 0
        z2 = self.size[2]
        if self.flip:
            z = self.size[2]
            z2 = 0
        glPushMatrix()
        glTranslatef(*self.center)
        if self.flip:
            glFrontFace(GL_CW)
            glTranslatef(0, 0, -self.size[2])
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glBegin(GL_QUADS)
        # Z- side
        glNormal3f(0, 0, -1)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], -self.size[1], z)
        glTexCoord2f(0, self.size[1] / 2)
        glVertex3f(-self.size[0], self.size[1], z)
        glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
        glVertex3f(self.size[0], self.size[1], z)
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(self.size[0], -self.size[1], z)
        glEnd()
        glBegin(GL_TRIANGLES)
        # X+ side
        norm = cross((self.size[0], -self.size[1], self.size[2]), (self.size[0] * 2, 0, 0))
        glNormal3f(*norm)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], self.size[1], z)
        glTexCoord2f(self.size[0] / 4, self.size[2] / 4)
        glVertex3f(0, 0, z2)
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(self.size[0], self.size[1], z)
        # Y+ side
        norm = cross((self.size[0], -self.size[1], self.size[2]), (0, self.size[1] * 2, 0))
        glNormal3f(*norm)
        glTexCoord2f(0, 0)
        glVertex3f(self.size[0], self.size[1], z)
        glTexCoord2f(self.size[1] / 4, self.size[2] / 4)
        glVertex3f(0, 0, z2)
        glTexCoord2f(self.size[1] / 2, 0)
        glVertex3f(self.size[0], -self.size[1], z)
        # X- side
        norm = cross((-self.size[0], self.size[1], self.size[2]), (-self.size[0] * 2, 0, 0))
        glNormal3f(*norm)
        glTexCoord2f(0, 0)
        glVertex3f(self.size[0], -self.size[1], z)
        glTexCoord2f(self.size[0] / 4, self.size[2] / 4)
        glVertex3f(0, 0, z2)
        glTexCoord2f(self.size[0] / 2, 0)
        glVertex3f(-self.size[0], -self.size[1], z)
        # Y- side
        norm = cross((-self.size[0], self.size[1], self.size[2]), (0, -self.size[1] * 2, 0))
        glNormal3f(*norm)
        glTexCoord2f(0, 0)
        glVertex3f(-self.size[0], -self.size[1], z)
        glTexCoord2f(self.size[1] / 4, self.size[2] / 4)
        glVertex3f(0, 0, z2)
        glTexCoord2f(self.size[1] / 2, 0)
        glVertex3f(-self.size[0], self.size[1], z)
        glEnd()
        if self.flip:
            glFrontFace(GL_CCW)
        glPopMatrix()


### The End ###

""" BZFlag.UI.Drawable.Box

Classes to draw the boxes in the world
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

class BoxSides(DisplayList):
    textureName = 'boxwall.png'
    def set(self, center, angle, size):
        self.center = center
        self.angle = angle
        self.size = size

    def drawToList(self):
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
    def set(self, center, angle, size):
        self.center = center
        self.angle = angle
        self.size = size

    def drawToList(self):
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


### The End ###

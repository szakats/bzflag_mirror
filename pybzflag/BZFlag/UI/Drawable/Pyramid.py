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
from __future__ import division
from DisplayList import *
from OpenGL.GL import *
from BZFlag.Vector import cross, normalize


class Pyramid(DisplayList):
    def __init__(self, center, angle, size, flip):
        self.uvMap = None

        # Change the pyramid's texture and mapping depending on its slope
        slope = size[2] / min(size[0], size[1])

        if slope > 10:
            # It's a very pointy pyramid - XZ and YZ planar texture mapping
            # This maps each side of the pyramid to a triangle based at the bottom
            # of the texture with its tip at the top-center. The bottom of the pyramid
            # is mapped to a the top-left 1/4 of the texture.
            self.textureName = 'pillar.jpeg'
            self.uvMap = ((0,   3/4),
                          (1/4, 3/4),
                          (1/4, 1),
                          (0,   1),
                          
                          (0, 0),
                          (0.5, 1),
                          (1, 0),
                          
                          (0, 0),
                          (0.5, 1),
                          (1, 0),

                          (0, 0),
                          (0.5, 1),
                          (1, 0),

                          (0, 0),
                          (0.5, 1),
                          (1, 0))

        else:
            # Default pyramid
            self.textureName = 'black_marble.jpeg'
            repeats = (size[0] / 4, size[1] / 4)

        if not self.uvMap:
            # Default UV map: XY planar
            self.uvMap = ((0, 0),
                          (0, repeats[1]),
                          repeats,
                          (repeats[0], 0),
                          
                          (0, repeats[1]),
                          (repeats[0]/2, repeats[1]/2),
                          repeats,
                          
                          repeats,
                          (repeats[0]/2, repeats[1]/2),
                          (repeats[0], 0),
                          
                          (repeats[0], 0),
                          (repeats[0]/2, repeats[1]/2),
                          (0, 0),
                          
                          (0, 0),
                          (repeats[0]/2, repeats[1]/2),
                          (0, repeats[1]))
                       
        DisplayList.__init__(self, center, angle, size, flip)

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
        glTexCoord2f(*self.uvMap[0])
        glVertex3f(-self.size[0], -self.size[1], z)
        glTexCoord2f(*self.uvMap[1])
        glVertex3f(-self.size[0], self.size[1], z)
        glTexCoord2f(*self.uvMap[2])
        glVertex3f(self.size[0], self.size[1], z)
        glTexCoord2f(*self.uvMap[3])
        glVertex3f(self.size[0], -self.size[1], z)
        glEnd()
        glBegin(GL_TRIANGLES)
        # X+ side
        norm = normalize(cross((self.size[0], -self.size[1], self.size[2]), (self.size[0] * 2, 0, 0)))
        glNormal3f(*norm)
        glTexCoord2f(*self.uvMap[4])
        glVertex3f(-self.size[0], self.size[1], z)
        glTexCoord2f(*self.uvMap[5])
        glVertex3f(0, 0, z2)
        glTexCoord2f(*self.uvMap[6])
        glVertex3f(self.size[0], self.size[1], z)
        # Y+ side
        norm = normalize(cross((self.size[0], -self.size[1], self.size[2]), (0, self.size[1] * 2, 0)))
        glNormal3f(*norm)
        glTexCoord2f(*self.uvMap[7])
        glVertex3f(self.size[0], self.size[1], z)
        glTexCoord2f(*self.uvMap[8])
        glVertex3f(0, 0, z2)
        glTexCoord2f(*self.uvMap[9])
        glVertex3f(self.size[0], -self.size[1], z)
        # X- side
        norm = normalize(cross((-self.size[0], self.size[1], self.size[2]), (-self.size[0] * 2, 0, 0)))
        glNormal3f(*norm)
        glTexCoord2f(*self.uvMap[10])
        glVertex3f(self.size[0], -self.size[1], z)
        glTexCoord2f(*self.uvMap[11])
        glVertex3f(0, 0, z2)
        glTexCoord2f(*self.uvMap[12])
        glVertex3f(-self.size[0], -self.size[1], z)
        # Y- side
        norm = normalize(cross((-self.size[0], self.size[1], self.size[2]), (0, -self.size[1] * 2, 0)))
        glNormal3f(*norm)
        glTexCoord2f(*self.uvMap[13])
        glVertex3f(-self.size[0], -self.size[1], z)
        glTexCoord2f(*self.uvMap[14])
        glVertex3f(0, 0, z2)
        glTexCoord2f(*self.uvMap[15])
        glVertex3f(-self.size[0], self.size[1], z)
        glEnd()
        if self.flip:
            glFrontFace(GL_CCW)
        glPopMatrix()


### The End ###

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
from OpenGL.GL.ARB.multitexture import *
from BZFlag.UI import GLExtension
from BZFlag.World import Scale
import random


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


class BoxTop(DisplayList):
    def __init__(self, box):
        self.box = box
        self.textureNames = ['concrete.jpeg']
        self.tex2Coords = ( (0,0), (1,0), (1,1), (0,1) )

        # If the box is fairly large and squareish, use a square oil-stain texture
        if box.size[0] > 10 and box.size[1] > 10 and box.size[0] / box.size[1] < 2 and box.size[1] / box.size[0] < 2:
            self.textureNames.append(random.choice(('oilstain_1.png',
                                                    'oilstain_2.png',
                                                    'oilstain_3.png')))

        # If it's about the same width as a tank, assume it's a bridge and put some tread stains on it
        if box.size[0] > Scale.TankWidth and box.size[0] < Scale.TankWidth * 4 and box.size[1] > Scale.TankLength * 2:
            self.textureNames.append('treadstain_1.png')
            self.tex2Coords = ( (0,0), (0,1), (1,1), (1,0) )
        if box.size[1] > Scale.TankWidth and box.size[1] < Scale.TankWidth * 4 and box.size[0] > Scale.TankLength * 2:
            self.textureNames.append('treadstain_1.png')
            self.tex2Coords = ( (0,0), (1,0), (1,1), (0,1) )
            
        DisplayList.__init__(self, box)
    
    def set(self, box):
        self.polygon = self.box.toPolygon()
        self.height = self.box.center[2] + self.box.size[2]

        try:
            self.render.textures[1].texEnv = GL_MODULATE
        except IndexError:
            pass

    def drawToList(self):
        glPushMatrix()
        glTranslatef(0, 0, self.height)
        glNormal3f(0, 0, 1)
        glBegin(GL_POLYGON)
        for i in xrange(4):
            vertex = self.polygon[i]
            tex2Coord = self.tex2Coords[i]
            glTexCoord2f(vertex[0] / 30, vertex[1] / 30)
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, tex2Coord[0], tex2Coord[1])
            glVertex2f(*vertex)
        glEnd()
        glPopMatrix()


class BoxBottom(DisplayList):
    textureName = 'concrete_underside.jpeg'
    
    def set(self, polygon, height):
        self.polygon = polygon
        self.polygon.reverse()
        self.height = height

    def drawToList(self):
        glPushMatrix()
        glTranslatef(0, 0, self.height)
        glNormal3f(0, 0, -1)
        glBegin(GL_POLYGON)
        for vertex in self.polygon:
            glTexCoord2f(vertex[0] / 30, vertex[1] / 30)
            glVertex2f(*vertex)
        glEnd()
        glPopMatrix()

### The End ###

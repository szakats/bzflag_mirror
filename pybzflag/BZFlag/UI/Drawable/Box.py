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

from __future__ import division
from DisplayList import *
from OpenGL.GL import *
from OpenGL.GL.ARB.multitexture import *
from BZFlag.UI import GLExtension
from BZFlag.World import Scale
import random


class BoxSides(DisplayList):
    def __init__(self, box):
        # Default wall
        # This is brick, so we want to make sure we don't end up
        # with a fractional number of bricks. This brick texture has
        # 5 bricks vertically and about 5 horizontally.
        self.textureNames = ["brick.jpeg", "wall_grime.jpeg"]
        numBricks = (int(box.size[0]),
                     int(box.size[1]),
                     int(box.size[2]))
        self.texRepeats = (numBricks[0] / 5,
                           numBricks[1] / 5,
                           numBricks[2] / 5)
        self.tex2Repeats = (box.size[0] / 20,
                            box.size[1] / 20,
                            1)

        # Bridges and platforms
        if box.size[2] < 10 and box.center[2] > 1:
            self.textureNames = ["platform_side.jpeg", "wall_grime.jpeg"]
            aspect = 8/10
            self.texRepeats = (box.size[0] / box.size[2] * 2 * aspect,
                               box.size[1] / box.size[2] * 2 * aspect,
                               1)
            self.tex2Repeats = (box.size[0] / 4,
                                box.size[1] / 4,
                                1)
        
        DisplayList.__init__(self, box)
               
    def set(self, box):
        self.center = box.center
        self.angle = box.angle
        self.size = box.size
        try:
            self.render.textures[1].texEnv = GL_MODULATE
        except IndexError:
            pass

    def drawToList(self):
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glBegin(GL_QUADS)
        # Y+ side
        glNormal3f(0, 1, 0)
        glTexCoord2f(0, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0)
        glVertex3f(-self.size[0], self.size[1], 0)
        glTexCoord2f(0, self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, self.tex2Repeats[2])
        glVertex3f(-self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[0], self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[0], self.tex2Repeats[2])
        glVertex3f(self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[0], 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[0], 0)
        glVertex3f(self.size[0], self.size[1], 0)
        # Y- side
        glNormal3f(0, -1, 0)
        glTexCoord2f(0, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        glTexCoord2f(0, self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, self.tex2Repeats[2])
        glVertex3f(self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[0], self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[0], self.tex2Repeats[2])
        glVertex3f(-self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[0], 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[0], 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        # X+ side
        glNormal3f(1, 0, 0)
        glTexCoord2f(0, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0)
        glVertex3f(self.size[0], self.size[1], 0)
        glTexCoord2f(0, self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, self.tex2Repeats[2])
        glVertex3f(self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[1], self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[1], self.tex2Repeats[2])
        glVertex3f(self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[1], 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[1], 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        # X- side
        glNormal3f(-1, 0, 0)
        glTexCoord2f(0, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        glTexCoord2f(0, self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, self.tex2Repeats[2])
        glVertex3f(-self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[1], self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[1], self.tex2Repeats[2])
        glVertex3f(-self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[1], 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[1], 0)
        glVertex3f(-self.size[0], self.size[1], 0)
        glEnd()
        glPopMatrix()


class BoxTop(DisplayList):
    textureNames = ['concrete.jpeg']

    def set(self, box):
        self.polygon = box.toPolygon()
        self.height = box.center[2] + box.size[2]

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
            glTexCoord2f(vertex[0] / 20, vertex[1] / 20)
            glVertex2f(*vertex)
        glEnd()
        glPopMatrix()


class BoxBottom(DisplayList):
    textureName = 'concrete.jpeg'
               
    def set(self, box):
        self.polygon = box.toPolygon()
        self.polygon.reverse()
        self.height = box.center[2]

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


class BoxDecal(DisplayList):
    textureName = 'oilstain_1.png'
               
    def set(self, box):
        self.polygon = box.toPolygon()
        self.height = box.center[2] + box.size[2]
        self.render.decal = True

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


## class BoxDecal(DisplayList):
##     def __init__(self, box):
##         self.box = box
##         self.textureNames = ['concrete.jpeg']
##         self.tex2Coords = ( (0,0), (1,0), (1,1), (0,1) )

##         # If the box is fairly large and squareish, use a square oil-stain texture
##         if box.size[0] > 10 and box.size[1] > 10 and box.size[0] / box.size[1] < 2 and box.size[1] / box.size[0] < 2:
##             self.textureNames.append(random.choice(('oilstain_1.png',
##                                                     'oilstain_2.png',
##                                                     'oilstain_3.png')))

##         # If it's about the same width as a tank, assume it's a bridge and put some tread stains on it
##         if box.size[0] > Scale.TankWidth and box.size[0] < Scale.TankWidth * 4 and box.size[1] > Scale.TankLength * 2:
##             self.textureNames.append('treadstain_1.png')
##             self.tex2Coords = ( (0,0), (0,1), (1,1), (1,0) )
##         if box.size[1] > Scale.TankWidth and box.size[1] < Scale.TankWidth * 4 and box.size[0] > Scale.TankLength * 2:
##             self.textureNames.append('treadstain_1.png')
##             self.tex2Coords = ( (0,0), (1,0), (1,1), (0,1) )
            
##         DisplayList.__init__(self, box)
    
##     def set(self, box):
##         self.polygon = self.box.toPolygon()
##         self.height = self.box.center[2] + self.box.size[2]

##         try:
##             self.render.textures[1].texEnv = GL_MODULATE
##         except IndexError:
##             pass

##     def drawToList(self):
##         glPushMatrix()
##         glTranslatef(0, 0, self.height)
##         glNormal3f(0, 0, 1)
##         glBegin(GL_POLYGON)
##         for i in xrange(4):
##             vertex = self.polygon[i]
##             glTexCoord2f(vertex[0] / 20, vertex[1] / 20)
##             tex2Coord = self.tex2Coords[i]
##             glMultiTexCoord2fARB(GL_TEXTURE1_ARB, tex2Coord[0], tex2Coord[1])
##             glVertex2f(*vertex)
##         glEnd()
##         glPopMatrix()

### The End ###

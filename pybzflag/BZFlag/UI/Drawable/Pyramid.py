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
from BZFlag import Vector
from OpenGL.GL.ARB.multitexture import *
from BZFlag.UI import CubeMap, GLExtension


class Pyramid(DisplayList):
    def __init__(self, pyramid):
        center = pyramid.center
        angle  = pyramid.angle
        size   = pyramid.size
        flip   = 'flipZ' in pyramid.options
        self.uvMap = None
        self.uvMap2 = None
        self.water = False

        # Change the pyramid's texture and mapping depending on its slope
        try:
            slope = size[2] / min(size[0], size[1])
        except ZeroDivisionError:
            slope = 1e100

        if slope > 10:
            # It's a very pointy pyramid - XZ and YZ planar texture mapping
            # This maps each side of the pyramid to a triangle based at the bottom
            # of the texture with its tip at the top-center. The bottom of the pyramid
            # is mapped to a the top-left 1/4 of the texture.
            self.textureNames = ['pillar.jpeg']
            if GLExtension.cubeMap:
                self.textureNames.append(CubeMap.CubeMap(Vector.add(center, (0,0,size[2]/2))))
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

        elif slope < 0.5 and center[2] > 0:
            # It's a very flat pyramid. Due to their blue color these are often
            # used to simulate water- so, let's make it actually look like water.
            # Texture this in world coordinates so all the water lines up.
            self.textureNames = ('caustic%02d.jpeg:50.0', 'water.jpeg')
            poly = pyramid.toPolygon()
            self.water = True

            def worldMap(scale):
                texCenter = ((poly[0][0] + poly[2][0]) / 2 / scale,
                             (poly[0][1] + poly[2][1]) / 2 / scale)
                return ((poly[0][0]/scale, poly[0][1]/scale),
                        (poly[1][0]/scale, poly[1][1]/scale),
                        (poly[2][0]/scale, poly[2][1]/scale),
                        (poly[3][0]/scale, poly[3][1]/scale),
                        
                        (poly[1][0]/scale, poly[1][1]/scale),
                        texCenter,
                        (poly[2][0]/scale, poly[2][1]/scale),
                        
                        (poly[2][0]/scale, poly[2][1]/scale),
                        texCenter,
                        (poly[3][0]/scale, poly[3][1]/scale),
                        
                        (poly[3][0]/scale, poly[3][1]/scale),
                        texCenter,
                        (poly[0][0]/scale, poly[0][1]/scale),
                        
                        (poly[0][0]/scale, poly[0][1]/scale),
                        texCenter,
                        (poly[1][0]/scale, poly[1][1]/scale))

            self.uvMap  = worldMap(20)
            self.uvMap2 = worldMap(200)
            
        else:
            # Default pyramid
            self.textureNames = ['black_marble.jpeg']
            if GLExtension.cubeMap:
                self.textureNames.append(CubeMap.CubeMap(Vector.add(center, (0,0,size[2]/2))))
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

        if not self.uvMap2:
            self.uvMap2 = self.uvMap
                       
        DisplayList.__init__(self, center, angle, size, flip)

    def set(self, center, angle, size, flip):
        self.center = center
        self.angle = angle
        self.size = size
        self.flip = flip
        try:
            self.render.textures[1].texEnv = GL_MODULATE
        except IndexError:
            pass
        if self.water:
            self.render.blended = True

    def drawToList(self, rstate):
        if self.render.blended:
            glColor4f(1, 1, 1, 0.6)
            glDisable(GL_LIGHTING)
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
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[0])
        glVertex3f(-self.size[0], -self.size[1], z)
        glTexCoord2f(*self.uvMap[1])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[1])
        glVertex3f(-self.size[0], self.size[1], z)
        glTexCoord2f(*self.uvMap[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[2])
        glVertex3f(self.size[0], self.size[1], z)
        glTexCoord2f(*self.uvMap[3])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[3])
        glVertex3f(self.size[0], -self.size[1], z)
        glEnd()
        glBegin(GL_TRIANGLES)
        # X+ side
        norm = Vector.normalize(Vector.cross((self.size[0], -self.size[1], self.size[2]), (self.size[0] * 2, 0, 0)))
        glNormal3f(*norm)
        glTexCoord2f(*self.uvMap[4])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[4])
        glVertex3f(-self.size[0], self.size[1], z)
        glTexCoord2f(*self.uvMap[5])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[5])
        glVertex3f(0, 0, z2)
        glTexCoord2f(*self.uvMap[6])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[6])
        glVertex3f(self.size[0], self.size[1], z)
        # Y+ side
        norm = Vector.normalize(Vector.cross((self.size[0], -self.size[1], self.size[2]), (0, self.size[1] * 2, 0)))
        glNormal3f(*norm)
        glTexCoord2f(*self.uvMap[7])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[7])    
        glVertex3f(self.size[0], self.size[1], z)
        glTexCoord2f(*self.uvMap[8])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[8])
        glVertex3f(0, 0, z2)
        glTexCoord2f(*self.uvMap[9])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[9])
        glVertex3f(self.size[0], -self.size[1], z)
        # X- side
        norm = Vector.normalize(Vector.cross((-self.size[0], self.size[1], self.size[2]), (-self.size[0] * 2, 0, 0)))
        glNormal3f(*norm)
        glTexCoord2f(*self.uvMap[10])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[10])
        glVertex3f(self.size[0], -self.size[1], z)
        glTexCoord2f(*self.uvMap[11])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[11])
        glVertex3f(0, 0, z2)
        glTexCoord2f(*self.uvMap[12])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[12])
        glVertex3f(-self.size[0], -self.size[1], z)
        # Y- side
        norm = Vector.normalize(Vector.cross((-self.size[0], self.size[1], self.size[2]), (0, -self.size[1] * 2, 0)))
        glNormal3f(*norm)
        glTexCoord2f(*self.uvMap[13])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[13])
        glVertex3f(-self.size[0], -self.size[1], z)
        glTexCoord2f(*self.uvMap[14])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[14])
        glVertex3f(0, 0, z2)
        glTexCoord2f(*self.uvMap[15])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, *self.uvMap2[15])
        glVertex3f(-self.size[0], self.size[1], z)
        glEnd()
        if self.flip:
            glFrontFace(GL_CCW)
        glPopMatrix()
        if self.render.blended:
            glEnable(GL_LIGHTING)
            glColor3f(1,1,1)

### The End ###

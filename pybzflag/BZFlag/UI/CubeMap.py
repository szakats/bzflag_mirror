""" BZFlag.UI.CubeMap

This module provides support routines for th OpenGL extension
EXT_TEXTURE_CUBE_MAP. This includes generating cube maps from
any point in the scene on the fly, and applying these cube maps
to rendered objects.

This module should only be used if GLExtension.cubeMap is True.
It is fine to import it whether or not cube maps are supported,
but the CubeMap class should not be instantiated.
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

from __future__ import division
from OpenGL.GL.EXT.texture_cube_map import *
from OpenGL.GL import *
from BZFlag.UI.Texture import Texture
import math


class CubeMap(Texture):
    """Abstraction for creating and using cube environment maps
       maxSize is the maximum height of the cube in texels.
       The actual height could be limited by viewport size.
       """
    def __init__(self, position=(0,0,0), maxSize=256):
        Texture.__init__(self, 'data/water.jpeg')
        
        self.position = position
        self.maxSize = maxSize
        self.target = GL_TEXTURE_CUBE_MAP_EXT
        self.texEnv = GL_REPLACE
        self.recursion = 0
        self.rendered = False
        self.viewport = None
        self.dirty = True

    def bind(self, rstate):
        """The first time we're bound we add a new viewport that renders our cube map"""
        if not self.viewport:
            self.rstate = rstate
            self.viewport = self.setupViewport()
        #Texture.bind(self, rstate)

    def getTextureRect(self, viewport):
        """Return a function that calculates the texture size taking into account
           our maximum texture size and the given root viewport size.
           """
        def fsize():
            viewMinorAxis = min(viewport.size[0], viewport.size[1])
            largestPowerOfTwo = pow(2, int(math.log(viewMinorAxis) / math.log(2)))
            size = self.maxSize
            if size > largestPowerOfTwo:
                size = largestPowerOfTwo
            return (0,0,size,size)
        return fsize

    def setupViewport(self):
        """Set up a viewport region in which this texture will be rendered,
           given the current root viewport.
           """
        region = self.rstate.viewport.region(
            self.getTextureRect(self.rstate.viewport), renderLink='before')
        region.onDrawFrame.observe(self.drawFrame)

        # We must have a 90-degree FOV to render cube maps
        region.fov = 90
        return region
        
    def drawFrame(self):
        """Draw function called by our viewport"""
        if not self.dirty:
            return
        
        glReadBuffer(GL_BACK)
        #Texture.bind(self)
        glTexParameteri(self.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR)

        self.renderColors()
        self.dirty = False


    def renderColors(self):
        """For debugging, render solid color to each side of the cube map"""
        for target, color in (
            (GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT, (1,0,0,1)),
            (GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT, (0,1,1,1)),
            (GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT, (0,1,0,1)),
            (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT, (1,0,1,1)),
            (GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT, (0,0,1,1)),
            (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT, (1,1,0,1)),
            ):
            glClearColor(*color)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)       
            glCopyTexImage2D(target, 0, GL_RGB,
                             self.viewport.rect[0],
                             self.viewport.rect[1],
                             self.viewport.rect[2],
                             self.viewport.rect[3],
                             0)
        glClearColor(0,0,0,1)

    def renderSides(self):
        """Render all six sides of the cube map"""
        cameraPos = (-self.position[0],
                     -self.position[1],
                     -self.position[2])

        glLoadIdentity()
        glRotatef(-180, 1,0,0)
        glTranslatef(*cameraPos)
        self.renderSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT)

        glLoadIdentity()
        glTranslatef(*cameraPos)
        self.renderSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT)

        glLoadIdentity()
        glRotatef(-90, 1,0,0)
        glTranslatef(*cameraPos)
        self.renderSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT)

        glLoadIdentity()
        glRotatef(-90, 1,0,0)
        glRotatef(180, 0,0,1)
        glTranslatef(*cameraPos)
        self.renderSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT)

        glLoadIdentity()
        glRotatef(-90, 1,0,0)
        glRotatef(90, 0,0,1)
        glTranslatef(*cameraPos)
        self.renderSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT)

        glLoadIdentity()
        glRotatef(-90, 1,0,0)
        glRotatef(270, 0,0,1)
        glTranslatef(*cameraPos)
        self.renderSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT)

    def renderSide(self, target):
        """Render one side of the cube map, storing it in the given texture target.
           The caller is responsible for positioning the camera correctly.
           """
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.rstate.view.renderScene(self.rstate)
        glCopyTexImage2D(target, 0, GL_RGB,
                         self.viewport.rect[0],
                         self.viewport.rect[1],
                         self.viewport.rect[2],
                         self.viewport.rect[3],
                         0)

### The End ###

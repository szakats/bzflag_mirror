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
from BZFlag.UI.Drawable.GLDrawable import GLDrawable
import math


# A global lock to keep us from rendering any cube maps while we're already rendering one.
# Not that recursive cube mapping isn't neat, but we are trying to do this in real time.
renderingCubeMap = False


class CubeMap(Texture, GLDrawable):
    """Abstraction for creating and using cube environment maps
       maxSize is the maximum height of the cube in texels.
       The actual height could be limited by viewport size.

       This class is both a texture and a drawable. It goes into
       a texture rendering pass that runs before anything else,
       where it renders the texture. Then later it can be used
       as a texture in one of the visible rendering passes.
       """
    def __init__(self, position=(0,0,0), maxSize=256):
        Texture.__init__(self)
        GLDrawable.__init__(self)
        
        self.position = position
        self.maxSize = maxSize
        self.target = GL_TEXTURE_CUBE_MAP_EXT
        self.recursion = 0
        self.rendered = False
    
    def draw(self, rstate):
        """Renders a cube map at the given position in world
           coordinates in the supplied ThreeDRender.View instance.
           This must be done in a rendering pass before everything
           else in the scene, as it uses the top-left corner of the
           backbuffer as temporary space.
           """
        return
        
        # Don't allow recursive rendering of cubemaps inside other cubemaps
        global renderingCubeMap
        if renderingCubeMap:
            return
        renderingCubeMap = True
        
        # Determine the actual size of our texture
        viewMinorAxis = min(rstate.viewport.size[0], rstate.viewport.size[1])
        largestPowerOfTwo = pow(2, int(math.log(viewMinorAxis) / math.log(2)))
        size = self.maxSize
        if size > largestPowerOfTwo:
            size = largestPowerOfTwo

        # Create a viewport we can render the texture into
        #texViewport = rstate.viewport.region((0, 0, size, size))
        #texViewport.configureOpenGL()

        #glLoadIdentity()
        #glTranslatef(*self.position)
        glClear(GL_DEPTH_BUFFER_BIT)
        rstate.view.renderScene()
        glClear(GL_DEPTH_BUFFER_BIT)
        rstate.viewport.display.flip()

        #self.rendered = True
        renderingCubeMap = False

### The End ###

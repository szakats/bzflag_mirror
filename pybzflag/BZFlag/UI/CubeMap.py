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

from OpenGL.GL.EXT.texture_cube_map import *
from OpenGL.GL import *
from BZFlag.UI.Texture import Texture


class CubeMap(Texture):
    """Abstraction for creating and using cube environment maps
       maxSize is the maximum height of the cube in texels.
       The actual height could be limited by viewport size.
       """
    def __init__(self, defaultPosition=(0,0,0), maxSize=256):
        self.defaultPosition = defaultPosition
        Texture.__init__(self)
        self.maxSize = maxSize
        self.rendered = False
        self.target = GL_TEXTURE_CUBE_MAP_EXT
    
    def render(self, view, position=None):
        """Renders a cube map at the given position in world
           coordinates in the supplied ThreeDRender.View instance.
           This must be done in a rendering pass before everything
           else in the scene, as it uses the top-left corner of the
           backbuffer as temporary space.
           """
        if not position:
            position = self.defaultPosition

        print "Render %r" % self

        self.rendered = True

    def bind(self, view):
        if not self.rendered:
            self.render(view)
        Texture.bind(self, view)

### The End ###

""" BZFlag.UI.Drawable.GLDrawable

Abstract base class for an object that can be drawn to an OpenGL context.
This is the basic unit used by the ThreeDView to sort objects by texture,
so it is required that the drawable have at most one texture.
The blended flag is used to put objects that need blending into a
second rendering pass.
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
from BZFlag.UI import Texture

__all__ = ['GLDrawable', 'RenderSettings']


class RenderSettings:
    """Settings that affect how ThreeDRender processes a a drawable"""
    def __init__(self):
        self.textures = []
        self.blended = False
        self.overlay = False
        self.static = True

    def __repr__(self):
        settings = [key for key in self.__dict__.iterkeys() if key[0] != '_']
        pairs = ["%s=%s" % (key, self.__dict__[key]) for key in settings] 
        return "<%s %s>" % (self.__class__.__name__, ", ".join(pairs))
        

class GLDrawable:
    """Abstract base class for an object that can be drawn to an OpenGL context.
       This is the basic unit used by the ThreeDView to sort objects by texture,
       so it is required that the drawable have at most one texture.
       The blended flag is used to put objects that need blending into a
       second rendering pass.
       """

    # The majority of object will only need one texture, so give them a simpler
    # syntax. Note that textureNames is for multitexturing. Textures are never
    # bound during the rendering of a drawable.
    textureName = None
    textureNames = []

    def __init__(self):
        self.render = RenderSettings()
	self.object = None
        self.loadTextures()

    def loadTextures(self):
        textures = []
        if self.textureName:
            textures.append(Texture.load(self.textureName))
        for name in self.textureNames:
            textures.append(Texture.load(name))
        self.render.textures = tuple(textures)

    def parent(self, parent):
        self.object = parent

    def draw(self):
        pass

    def drawToList(self):
        """An alternate drawing function may be provided
           for using this drawable inside a display list
           """
        self.draw()

### The End ###

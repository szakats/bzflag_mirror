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

__all__ = ['GLDrawable']

class GLDrawable:
    """Abstract base class for an object that can be drawn to an OpenGL context.
       This is the basic unit used by the ThreeDView to sort objects by texture,
       so it is required that the drawable have at most one texture.
       The blended flag is used to put objects that need blending into a
       second rendering pass.
       """
    textureName = None

    def __init__(self):
        if self.textureName:
            self.texture = Texture.load(self.textureName)
        else:
            self.texture = None
        self.blended = False
        self.overlay = False

    def draw(self):
        pass


### The End ###

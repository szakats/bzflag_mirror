""" BZFlag.UI.Drawable.Container

Basic containers that can be used to group together multiple drawables
into one, and to perform transformations on other drawables.
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
from BZFlag.UI.Drawable.GLDrawable import *
from BZFlag.UI.Drawable.DisplayList import *
from OpenGL.GL import *

__all__ = ['Group', 'Transformer', 'Transform', 'Colorize', 'Position', 'Rotate']


class Group(DisplayList):
    """When several drawables are generally used together, this drawable
       can be used to group them all together into a single display list.

       This may only be used with drawables that would be placed in the same rendering pass,
       and have the same texture.
       """
    def set(self, children):
        self.texture = children[0].texture
        self.children = children

    def drawToList(self):
        for child in self.children:
            child.drawToList()


class Transformer(GLDrawable):
    """Holds one drawable, applying transforms before rendering it.
       To transform several drawables using the same settings, wrap each drawable with a
       Transformer but point them all to one central Transform list.
       """
    def __init__(self, child, transforms):
        GLDrawable.__init__(self)
        self.child = child
        self.transforms = transforms

    def draw(self):
        glPushMatrix()
        for transform in self.transforms:
            transform.apply()
        self.child.draw()
        glColor3f(1,1,1)
        glPopMatrix()
        

class Transform:
    """Abstract base class for transforms designed for use with Transformer"""
    def apply(self):
        """Apply this transform to the current OpenGL state"""
        pass


class Colorize(Transform):
    """Changes the current color"""
    def __init__(self, rgb=[1,1,1]):
        self.rgb = rgb

    def apply(self):
        glColor3f(*self.rgb)


class Position(Transform):
    """Move the drawable's origin"""
    def __init__(self, vector=[0,0,0]):
        self.vector = vector

    def apply(self):
        glTranslatef(*self.vector)


class Rotate(Transform):
    """Rotate the drawable using Euler angles"""
    def __init__(self, angle=[0,0,0]):
        self.angle = angle

    def apply(self):
        glRotatef(self.angle[2], 0,0,1)
        glRotatef(self.angle[0], 1,0,0)
        glRotatef(self.angle[1], 0,1,0)


### The End ###

""" BZFlag.UI.Drawable.DisplayList

A drawable that stores itself to a display list before rendering
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
from OpenGL.GL import *

class DisplayList(GLDrawable):
    """A drawable that stores itself to a display list before rendering"""
    def __init__(self, *args, **kw):
        GLDrawable.__init__(self)
        self.list = glGenLists(1)
        self.set(*args, **kw)
        self.init()

    def set(self):
        """Subclasses should use this to change the drawable's state.
           It is called before init(), with the same parameters the class
           was constructed with.
           """
        pass

    def init(self):
        """Called on init after setting up the display list. By default this
           builds the display lists, but this hook lets subclasses override that.
           """
        self.buildList()

    def __del__(self):
        try:
            glDeleteLists(self.list, 1)
        except:
            pass

    def buildList(self):
        """Rebuild this object's display list."""
        glNewList(self.list, GL_COMPILE)
        self.drawToList()
        glEndList()

    def drawToList(self):
        """Stub where subclasses will do their drawing"""
        pass

    def draw(self):
        glCallList(self.list)


### The End ###

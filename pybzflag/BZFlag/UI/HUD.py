""" BZFlag.UI.HUD

Views and utilities for rendering the Heads Up Display
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

from OpenGL.GL import *


class Panel:
    """A view that draws a shaded background and thin border"""
    def __init__(self, viewport):
        viewport.fov = None
        self.viewport = viewport
        viewport.onDrawFrame.observe(self.render)

    def render(self):
        glEnable(GL_BLEND)
        glDisable(GL_LIGHTING)
        glDisable(GL_CULL_FACE)
        glDisable(GL_COLOR_MATERIAL)
        glDisable(GL_DEPTH_TEST)
        glDisable(GL_TEXTURE_2D)
        glLoadIdentity()

        size = self.viewport.size
            
        def square():
            # Set the square inside our viewport by one
            # pixel so the antialiased lines won't be chopped in half.
            glVertex2f(1,1)
            glVertex2f(size[0]-1, 1)
            glVertex2f(size[0]-1, size[1]-1)
            glVertex2f(1, size[1]-1)

        # Creamy translucent filling
        glColor4f(0,0,0,0.3)
        glBegin(GL_POLYGON)
        square()
        glEnd()

        # Bright border
        glColor4f(1,1,1,1)
        glBegin(GL_LINE_LOOP)
        square()
        glEnd()


### The End ###

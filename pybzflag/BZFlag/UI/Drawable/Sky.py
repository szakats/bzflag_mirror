""" BZFlag.UI.Drawable.Sky

Drawables used in creating PyBZFlag's sky. These drawables are all
used in BZFlag.UI.Environment.Sky
"""
#
# Python BZFlag Package
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

from DisplayList import DisplayList
from OpenGL.GL import *
from OpenGL.GLU import *
import VRML
from BZFlag import Animated


class Colors(DisplayList):
    """The sky itself, with colors changing over the course of the day"""
    textureName = 'sky_colors.png'
    def set(self, sky):
        self.sky = sky
        
        # We render in a z-buffer-isolated pass before all other geometry.
        # This means that we can work with the sky on a scale independent of the rest of the world.
        self.render.background = True

        # The mesh itself is in a display list, but this drawable needs to update the texture
        # coordinates each frame, so don't cache this drawable in the big render pass display lists
        self.render.static = False

    def drawToList(self, rstate):
        """Our display list only holds an inverted sphere and the static
           texture coordinates. Texture coordinates must be adjusted for time
           of day in draw().
           """
        # Texture the top half of the sky sphere with our changing gradient,
        # leaving the bottom half clamped to the horizon.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)       
        glDisable(GL_LIGHTING)
        glTexGenfv(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
        glTexGenfv(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
        glTexGenfv(GL_T, GL_OBJECT_PLANE, (0, 0, 0.1, 0))
        glEnable(GL_TEXTURE_GEN_S)
        glEnable(GL_TEXTURE_GEN_T)

        quad = gluNewQuadric()
        gluQuadricOrientation(quad, GLU_INSIDE)
        gluSphere(quad, 10, 8, 8)

        glDisable(GL_TEXTURE_GEN_S)
        glDisable(GL_TEXTURE_GEN_T)
        glEnable(GL_LIGHTING)

    def draw(self, rstate):
        # Use texture coordinate generation to set the time of day.
        # The X axis of our texture is time, representing one day. The Y axis is spacial,
        # stretching from the bottom of the sky sphere to the top.
        glTexGenfv(GL_S, GL_OBJECT_PLANE, (0, 0, 0, self.sky.unitDayTime))
        DisplayList.draw(self, rstate)

### The End ###

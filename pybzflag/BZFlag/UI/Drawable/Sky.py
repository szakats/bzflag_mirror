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
from BZFlag import Animated
from BZFlag.UI import GLNoise
import VRML


class SkyDrawable(DisplayList):
    """Drawable functionality common to all elements of the sky"""
    def set(self, sky):
        self.sky = sky

        # We render in a z-buffer-isolated pass before all other geometry.
        # This means that we can work with the sky on a scale independent of the rest of the world.
        self.render.background = True

        # The mesh itself is in a display list, but this drawable needs to update the texture
        # coordinates each frame, so don't cache this drawable in the big render pass display lists
        self.render.static = False


class Colors(SkyDrawable):
    """The sky itself, with colors changing over the course of the day"""
    textureName = 'sky_colors.png'
    def drawToList(self, rstate):
        """Our display list only holds an inverted sphere and the static
           texture coordinates. Texture coordinates must be adjusted for time
           of day in draw().
           """
        # Texture the sky dome vertically with the Y axis of our gradient.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)
        glTexGenfv(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
        glTexGenfv(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
        glTexGenfv(GL_T, GL_OBJECT_PLANE, (0, 0, 1, 0))
        glEnable(GL_TEXTURE_GEN_S)
        glEnable(GL_TEXTURE_GEN_T)

        VRML.load('sky.wrl')['dome'].drawToList(rstate)

        glDisable(GL_TEXTURE_GEN_S)
        glDisable(GL_TEXTURE_GEN_T)

    def draw(self, rstate):
        # Use texture coordinate generation to set the time of day.
        # The X axis of our texture is time, representing one day. The Y axis is spacial,
        # stretching from the bottom of the sky sphere to the top.
        glTexGenfv(GL_S, GL_OBJECT_PLANE, (0, 0, 0, self.sky.unitDayTime))
        DisplayList.draw(self, rstate)


class Clouds(SkyDrawable):
    """Dynamic perlin-noise-based clouds"""
    def __init__(self, *args, **kw):
        SkyDrawable.__init__(self, *args, **kw)
        self.render.textures = (GLNoise.CloudTexture(),)
        self.motion = Animated.Value(Animated.RampFunction(200))
        self.time = Animated.Timekeeper()
        self.scale = 0.2

    def drawToList(self, rstate):
        """Do all the drawing we can in the display list"""

        # Set up blending to use only the texture's alpha
        # channel, disregarding its color channels
        glColor3f(1,1,1)
        glEnable(GL_BLEND)
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)

        # Set up texture coordinate generation. The plane equations
        # are set up every frame in draw() to animate the clouds' motion
        glTexGenfv(GL_T, GL_OBJECT_PLANE, (0, self.scale, 0, 0))
        glTexGenfv(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
        glTexGenfv(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
        glEnable(GL_TEXTURE_GEN_S)
        glEnable(GL_TEXTURE_GEN_T)

        VRML.load('sky.wrl')['dome'].drawToList(rstate)

        # Cleanup!
        glDisable(GL_TEXTURE_GEN_S)
        glDisable(GL_TEXTURE_GEN_T)
        glDisable(GL_BLEND)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    def draw(self, rstate):
        """Animate the texture coordinates every frame"""
        self.motion.integrate(self.time.step())
        glTexGenfv(GL_S, GL_OBJECT_PLANE, (self.scale, 0, 0, self.motion.value))
        DisplayList.draw(self, rstate)


class Horizon(SkyDrawable):
    """Some mountains and a chasm to cover up the horizon"""
    def drawToList(self, rstate):
        glEnable(GL_BLEND)

        # Set up texture coordinate generation. The plane equations
        # are set up every frame in draw() to animate the clouds' motion
        glTexGenfv(GL_S, GL_OBJECT_PLANE, (1, 0, 0, 0))
        glTexGenfv(GL_T, GL_OBJECT_PLANE, (0, 0, 1, 0))
        glTexGenfv(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
        glTexGenfv(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
        glEnable(GL_TEXTURE_GEN_S)
        glEnable(GL_TEXTURE_GEN_T)

        VRML.load('sky.wrl')['horizon'].drawToList(rstate)

        # Cleanup!
        glDisable(GL_TEXTURE_GEN_S)
        glDisable(GL_TEXTURE_GEN_T)
        glDisable(GL_BLEND)

### The End ###

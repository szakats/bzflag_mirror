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

from __future__ import division
from DisplayList import DisplayList
from OpenGL.GL import *
from OpenGL.GLU import *
from BZFlag import Animated, Noise
from BZFlag.UI import GLNoise, Texture
import VRML, math
from Numeric import *


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
    def __init__(self, sky):
        SkyDrawable.__init__(self, sky)
        self.render.textures[0].setRepeat(GL_REPEAT, GL_CLAMP)

    def drawToList(self, rstate):
        """Our display list only holds an inverted sphere and the static
           texture coordinates. Texture coordinates must be adjusted for time
           of day in draw().
           """
        # Texture the sky dome vertically with the Y axis of our gradient.
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


class CloudTexture(GLNoise.MappedPerlinTexture):
    """Perlin noise with mapping applied to create a cloud-like texture"""
    def map(self, y):
        try:
            return math.log((y-0.35) * 40 + 1) * 0.27
        except ValueError:
            return 0


class MountainTexture(Texture.DynamicTexture):
    """Perlin-noise-based mountain range silhouette"""
    def __init__(self, size=(512,32), noise=None):
        if not noise:
            noise = Noise.PerlinNoise(1, octaves=12, persistence=0.4)
        self.noise = noise
        Texture.DynamicTexture.__init__(self, size)
        self.setRepeat(GL_REPEAT, GL_CLAMP)
        self.format = GL_INTENSITY8

    def draw(self):
        # To get the most quality for our texel, sample the perlin noise twice per texel.
        samples = arange(0, 1, 1/(self.viewport.size[0] * 2))
        heights = self.noise.get(samples[...,NewAxis])

        glClearColor(0,0,0,1)
        glClear(GL_COLOR_BUFFER_BIT)
        glDisable(GL_DEPTH_TEST)
        glDisable(GL_CULL_FACE)
        glDisable(GL_LIGHTING)
        glEnable(GL_BLEND)
        glEnable(GL_LINE_SMOOTH)
        glColor3f(1,1,1)

        def vertices():
            for i in xrange(len(samples)):
                glVertex2f(samples[i] * self.viewport.size[0],
                           (-heights[i]/2 +0.5) * self.viewport.size[1])

        # The mountains themselves
        glBegin(GL_POLYGON)
        glVertex2f(0,self.viewport.size[1])
        vertices()
        glVertex2f(self.viewport.size[0],self.viewport.size[1])
        glEnd()

        # Smoothed outline
        glBegin(GL_LINE_STRIP)
        vertices()
        glEnd()

        glDisable(GL_BLEND)
        glDisable(GL_LINE_SMOOTH)


class Clouds(SkyDrawable):
    """Dynamic perlin-noise-based clouds"""
    def __init__(self, *args, **kw):
        SkyDrawable.__init__(self, *args, **kw)
        self.render.textures = (CloudTexture(),)
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
    def __init__(self, *args, **kw):
        SkyDrawable.__init__(self, *args, **kw)
        self.render.textures = (MountainTexture(),)

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

        # FIXME: the color of the mountains
        glColor3f(0,0,0.25)

        VRML.load('sky.wrl')['horizon'].drawToList(rstate)

        # Cleanup!
        glDisable(GL_TEXTURE_GEN_S)
        glDisable(GL_TEXTURE_GEN_T)
        glDisable(GL_BLEND)
        glColor3f(1,1,1)

### The End ###

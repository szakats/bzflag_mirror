""" BZFlag.UI.GLNoise

Noise rendering algorithms, hardware accelerated by OpenGL.
This module is only for algorithms that require OpenGL. General-purpose
noise algorithms should go in BZFlag.Noise.

The functions here require an ortho mode viewport to be active, and
generally assume they have free reign over the OpenGL backbuffer.
It is assumed that they will be invoked from a dynamic texture class
that creates its own viewport.
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

from __future__ import division
from BZFlag.UI import Texture
from BZFlag import Noise
from Numeric import *
from OpenGL.GL import *


def drawTexRect(destSize, texSize=(1,1)):
    """Draw a rectangle at the origin using the given sizes in texels and pixels"""
    glBegin(GL_QUADS)
    glTexCoord2f(0,0)
    glVertex2f(0,0)
    glTexCoord2f(texSize[0],0)
    glVertex2f(destSize[0],0)
    glTexCoord2f(*texSize)
    glVertex2f(*destSize)
    glTexCoord2f(0,texSize[1])
    glVertex2f(0,destSize[1])
    glEnd()


class NoiseTexture(Texture.Texture):
    """A texture filled with unfiltered random noise"""
    def __init__(self, size=(256,256), seed=None):
        Texture.Texture.__init__(self)
        r = Noise.randomArray(size, seed, (0,255), UInt8)
        self.loadRaw(size, r.tostring(), GL_LUMINANCE, 1)
        self.setFilter(GL_NEAREST, GL_NEAREST)


class NoiseRenderer:
    """Renders a NoiseTexture with scaling and rotation to
       generate a stream of random frames from one source texture.

       texture - The texture to scale and rotate. Generally this should be a
                 NoiseTexture instance with custom settings.

       scale - Texture coordinate scale

       rotationDistance - The distance away from the origin to put the center of rotation

       rotationSpeed - Number of degrees to advance the rotation every time a frame is generated
       """
    def __init__(self, texture=None, scale=50, rotationDistance=100, rotationSpeed=129):
        if not texture:
            texture = NoiseTexture()
        self.texture = texture
        self.scale = scale
        self.rotationDistance = rotationDistance
        self.rotationSpeed = rotationSpeed
        self.reset()

    def reset(self):
        self.angle = 0

    def render(self, texture, size):
        """Render noise into the given texture. This assumes an ortho mode viewport"""

        # Prepare our texture
        glEnable(GL_TEXTURE_2D)
        glDisable(GL_LIGHTING)
        glDisable(GL_BLEND)
        glColor3f(1,1,1)
        self.texture.bind()
        
        # Rotate the texture matrix
        glMatrixMode(GL_TEXTURE)
        glLoadIdentity()
        self.angle += self.rotationSpeed
        glRotatef(self.angle, 0,0,1)
        glTranslatef(-self.rotationDistance, -self.rotationDistance, 0)
        glMatrixMode(GL_MODELVIEW)

        drawTexRect(size, (self.scale, self.scale))
        
        # Clean up
        glMatrixMode(GL_TEXTURE)
        glLoadIdentity()
        glMatrixMode(GL_MODELVIEW)
        glDisable(GL_TEXTURE_2D)

        # Capture our rendered image
        texture.loadBackbuffer(size)


class AnimatedNoise:
    """Creates multiple frames of noise at the same size, and smoothly blends between them.
       Note that this isn't Perlin noise yet, just uncorrelated noise at a single resolution.
       Multiple AnimatedNoise instances are used to animate Perlin noise.

       This creates numFrames noise frames using the provided renderer at the given size.
       They are animated over the given period, in seconds.
       """
    def __init__(self, size, numFrames, period=1, renderer=None):
        if not renderer:
            renderer = NoiseRenderer()
            
        self.period = period
        self.size = size
        self.frameDuration = period / numFrames
        
        # Render and store all the frames we'll need
        self.frames = []
        for i in xrange(numFrames):
            frame = Texture.Texture()
            renderer.render(frame, size)
            self.frames.append(frame)

        # To prevent special cases in looping below, refer back to the first frame after the last frame
        self.frames.append(self.frames[0])

        self.reset()

    def reset(self):
        self.time = 0

    def integrate(self, dt):
        """Advance the clock by the given number of seconds"""
        self.time += dt
        self.time %= self.period

    def draw(self, size=None, color=(1,1,1,1)):
        """Draw the noise, optionally scaled to the given size and modulated by the given color"""
        if not size:
            size = self.frames[0].size

        glEnable(GL_BLEND)

        # To smoothly blend between two frames, we will need to know which two frames
        # we're blending between, and the amount of fading between them. The fade value
        # will be 0 for 100% frame1, and 1 for 100% frame2.
        frameNum = int(self.time / self.frameDuration)
        frame1 = self.frames[frameNum]
        frame2 = self.frames[frameNum+1]
        fade = (self.time % self.frameDuration) / self.frameDuration

        # To properly blend between the existing data in the color buffer and the
        # two frames, we need to do the blend operation in three passes.

        # 1. Multiply the color buffer by (1-alpha)
        glDisable(GL_TEXTURE_2D)
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA)
        glColor4f(1,1,1,color[3])
        drawTexRect(size)

        # 2. Multiply the first frame by (alpha * (1-fade)) and add it to the color buffer
        glEnable(GL_TEXTURE_2D)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE)
        glColor4f(color[0], color[1], color[2], color[3] * (1-fade))
        frame1.bind()
        drawTexRect(size)

        # 3. Multiply the second frame by (alpha * fade) and add it to the color buffer
        glColor4f(color[0], color[1], color[2], color[3] * fade)
        frame2.bind()
        drawTexRect(size)

        glDisable(GL_BLEND)
        glDisable(GL_TEXTURE_2D)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)


class AnimatedPerlinNoise:
    """Multiple layers of AnimatedNoise blended together into Perlin noise.
       - 'period' is the total length of the animation in seconds
       - 'persistence' controls the weighting of the octaves during blending
       - 'numOctaves' sets the number of noise octaves to animate. Time increases
         linearly with the number of octaves, memory increases exponentially.
       - 'fundamental' is the fundamental frequency of the noise in texels
       - 'framesPerOctave' sets the number of noise frames to render at each octave.
         More frames will produce a more detailed animation at the expense of memory.
       - 'renderer' is a NoiseRenderer instance to use. If None, one will be created.
       """
    def __init__(self,
                 period          = 50,
                 persistence     = 0.65,
                 numOctaves      = 8,
                 fundamental     = 2,
                 framesPerOctave = 32,
                 renderer        = None):
        if not renderer:
            renderer = NoiseRenderer()
        self.period = period
        self.persistence = persistence
        self.octaves = []
        self.numOctaves = numOctaves
        self.fundamental = fundamental
        self.framesPerOctave = framesPerOctave
        self.renderer = renderer

    def draw(self, size):
        """Draw the octaves, largest first, with alpha blending.
           This will correctly make the most prominent octaves the
           ones with the least detail.
           """
        if not self.octaves:
            self.createOctaves()
        color = (1,1,1,1)
        for octave in self.octaves:
            octave.draw(size, color)
            color = (1,1,1, 1 - self.persistence)

    def integrate(self, dt):
        """Advance the animation by 'dt' seconds"""
        for octave in self.octaves:
            octave.integrate(dt)

    def createOctaves(self):
        """Generate all octave textures"""
        self.octaves = []
        period = self.period
        size = self.fundamental
        for i in xrange(self.numOctaves):
            self.octaves.insert(0, AnimatedNoise((size,size), self.framesPerOctave, period, self.renderer))
            period /= 2
            size   *= 2
    
### The End ###


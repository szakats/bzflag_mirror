""" BZFlag.UI.GLNoise

Noise rendering algorithms, hardware accelerated by OpenGL.
This module is only for algorithms that require OpenGL. General-purpose
noise algorithms should go in BZFlag.Noise.
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

from BZFlag.UI import Texture
from BZFlag import Noise
from Numeric import *
from OpenGL.GL import *


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

        # Draw a unit square
        glBegin(GL_QUADS)
        glTexCoord2f(0,0)
        glVertex2f(0,0)
        glTexCoord2f(self.scale,0)
        glVertex2f(size[0],0)
        glTexCoord2f(self.scale,self.scale)
        glVertex2f(*size)
        glTexCoord2f(0,self.scale)
        glVertex2f(0,size[1])
        glEnd()

        # Clean up
        glMatrixMode(GL_TEXTURE)
        glLoadIdentity()
        glMatrixMode(GL_MODELVIEW)
        glDisable(GL_TEXTURE_2D)

        # Capture our rendered image
        texture.loadBackbuffer(size)

### The End ###


""" BZFlag.UI.Drawable.Particle

This drawable provides a simple interface for drawing textured particles from
a Numeric array of points. The underlying implementation can change to support
acceleration via OpenGL extensions like NV_point_sprite.
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
from Array import *
from GLDrawable import *
from OpenGL.GL import *
from OpenGL.GL.EXT.point_parameters import *
from OpenGL.GL.ARB.multitexture import *
from BZFlag.Geometry import *
from Numeric import *
from BZFlag.UI import GLExtension

# PyOpenGL doesn't yet have a module for NV_point_sprite
GL_POINT_SPRITE_NV        = 0x8861
GL_COORD_REPLACE_NV       = 0x8862
GL_POINT_SPRITE_R_MODE_NV = 0x8863


__all__ = ('ParticleArray',)


class ParticleArray(GLDrawable):
    """A drawable that picks a ParticleRenderer implementation at runtime.
       The goal was to hide most of the differences between software billboarding
       and extensions like NV_point_sprite, but that doesn't turn out to be practical.
       Point sprites will only be used if allowPointSprite is set and they are available.
       """
    def __init__(self, shape, pointDiameter, allowPointSprite=False):
        GLDrawable.__init__(self)

        if GLExtension.nvPointSprite and allowPointSprite:
            # Do hardware accelerated billboarding via NV_point_sprite
            # To get hardware acceleration on the GeForce 3, we have to
            # use texture unit 3 rather than 0... odd, eh?
            self.render.textures = (None,None,None) + self.render.textures
            self.renderer = PointSpriteRenderer(shape, pointDiameter)
        else:
            self.renderer = SoftwareParticleRenderer(shape, pointDiameter)

        # Reference public render state
        self.vertices = self.renderer.points
        self.colors = self.renderer.pointColors

    def setPointDiameter(self, d):
        self.renderer.pointDiameter = d

    def getPointDiameter(self, d):
        return self.renderer.pointDiameter

    def draw(self, rstate):
        self.renderer.draw(rstate)


class SoftwareParticleRenderer(VertexArray):
    """A ParticleRenderer that does all billboarding in software"""
    def __init__(self, shape, pointDiameter):
        self.pointDiameter = pointDiameter

        # This array format is somewhat wasteful, but we need at least T2F, C4F, and V3F :(
        # We need room for one quad per particle.
        VertexArray.__init__(self, shape + (4,), GL_T2F_C4F_N3F_V3F)
        self.numVertices = multiply.reduce(self.shape)

        # Initialize texture coordinates. These are static
        self.texcoords[...,0,:] = (0,1)
        self.texcoords[...,1,:] = (1,1)
        self.texcoords[...,2,:] = (1,0)
        self.texcoords[...,3,:] = (0,0)

        # Create point color and position arrays
        self.points      = zeros(shape + (3,), Float32, savespace=True)
        self.pointColors = ones(shape + (4,), Float32, savespace=True)

    def draw(self, rstate):
        # Stretch our point colors over each whole quad
        self.colors[...,0,:] = self.pointColors
        self.colors[...,1,:] = self.pointColors
        self.colors[...,2,:] = self.pointColors
        self.colors[...,3,:] = self.pointColors

        # Billboard ourselves some quads from the point positions and sizes
        radius = self.pointDiameter/2
        modelview = glGetFloatv(GL_MODELVIEW_MATRIX).flat
        up    = (take(modelview, (1,5,9)) * radius).astype(Float32)
        right = (take(modelview, (0,4,8)) * radius).astype(Float32)
        self.vertices[...,0,:] = self.points - right - up;
        self.vertices[...,1,:] = self.points + right - up;
        self.vertices[...,2,:] = self.points + right + up;
        self.vertices[...,3,:] = self.points - right + up;

        self.bind()
        glDrawArrays(GL_QUADS, 0, self.numVertices)


class PointSpriteRenderer(VertexArray):
    """A ParticleRenderer that uses the NV_point_sprite extension to accelerate billboarding"""
    def __init__(self, shape, pointDiameter):
        self.pointDiameter = pointDiameter

        VertexArray.__init__(self, shape, GL_C4F_N3F_V3F)
        self.numVertices = multiply.reduce(self.shape)

        # Our point and pointcolors arrays are just references into the VertexArray
        self.points      = self.vertices
        self.pointColors = self.colors

        # Initialize all our colors to one
        self.colors[...] = ones(self.colors.shape, Float32)

    def draw(self, rstate):
        glEnable(GL_POINT_SPRITE_NV)
        glPointSize(self.pointDiameter)

        ## FIXME: This isn't correct. It's still yet to be determined whether
        ##        it's possible to get the same size function that normal geometry
        ##        uses. There also appears to be a maximum point sprite size of 64 pixels
        ##        on my card, which just won't do for some uses of particles.
        #f = 1 / tan(rstate.viewport.fov / 2)
        #glPointParameterfvEXT(GL_POINT_DISTANCE_ATTENUATION_EXT, (0.1, 0, 0))
        #glPointParameterfEXT(GL_POINT_SIZE_MIN_EXT, 0)
        #glPointParameterfEXT(GL_POINT_SIZE_MAX_EXT, 8192)

        # Activate GL_COORD_REPLACE_NV only on texture unit 3. According to the
        # implementation notes, this is the only way it will actually be hardware
        # accelerated on the GeForce 3. ParticleArray helps us out by sticking
        # None in texture units 0 through 2.
        glActiveTextureARB(GL_TEXTURE3_ARB)
        glTexEnvi(GL_POINT_SPRITE_NV, GL_COORD_REPLACE_NV, GL_TRUE)

        self.bind()
        glDrawArrays(GL_POINTS, 0, self.numVertices)

        glTexEnvi(GL_POINT_SPRITE_NV, GL_COORD_REPLACE_NV, GL_FALSE)
        glActiveTextureARB(GL_TEXTURE0_ARB)
        glDisable(GL_POINT_SPRITE_NV)

### The End ###

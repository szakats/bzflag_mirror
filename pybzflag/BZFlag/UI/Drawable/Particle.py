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
from Array import *
from GLDrawable import *
from OpenGL.GL import *
from BZFlag.Geometry import *
from Numeric import *
from BZFlag.UI import GLExtension

__all__ = ('ParticleArray',)


class ParticleArray(GLDrawable):
    """A drawable that picks a ParticleRenderer implementation at runtime."""
    def __init__(self, shape, pointDiameter):
        GLDrawable.__init__(self)

        # Fall back to doing everything ourselves
        self.renderer = SoftwareParticleRenderer(shape, pointDiameter)

        # Reference public render state
        self.points = self.renderer.points
        self.pointColors = self.renderer.pointColors
        self.pointDiameter = self.renderer.pointDiameter

    def setPointDiameter(self, d):
        self.renderer.pointDiameter = d
        self.pointDiameter = d

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
        self.texcoords[...,0,:] = (0,0)
        self.texcoords[...,1,:] = (1,0)
        self.texcoords[...,2,:] = (1,1)
        self.texcoords[...,3,:] = (0,1)

        # Create point color and position arrays
        self.points      = zeros(shape + (3,), Float32)
        self.pointColors = ones(shape + (4,), Float32)

    def draw(self, rstate):
        # Stretch our point colors over each whole quad
        self.colors[...,0,:] = self.pointColors
        self.colors[...,1,:] = self.pointColors
        self.colors[...,2,:] = self.pointColors
        self.colors[...,3,:] = self.pointColors

        self.bind()
        glDrawArrays(GL_QUADS, 0, self.numVertices)

### The End ###

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
from OpenGL.GL import *
from BZFlag.Geometry import *
from Numeric import *
from BZFlag.UI import GLExtension

__all__ = ('ParticleArray', )


class AbstractParticleArray(GLDrawable):
    """Abstract base class for a particle drawable implementation.
       The particle array's shape is always held in the 'vertices' attribute.
       Its shape is the caller-specified shape with (3,) appended.
       This is consistent with the use of 'vertices

    def  __init__(self, shape):




class TriangleArray(VertexArray, GLDrawable):
    """A drawable which structures its arrays in groups of three, and draws them as
       unconnected triangles. This is best for meshes that can't be easily decomposed
       into triangle strips or other larger primitives.
       """
    def __init__(self, size, format):
        super(TriangleArray, self).__init__((size, 3), format)

    def draw(self, rstate):
        self.bind()
        glDrawArrays(GL_TRIANGLES, 0, self.shape[0] * 3)

### The End ###

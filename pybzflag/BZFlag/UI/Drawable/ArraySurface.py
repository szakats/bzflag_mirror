""" BZFlag.UI.Drawable.ArraySurface

A Drawable for surfaces generated dynamically from a 2D Numeric
array of vertex coordinates. Vertex normals are generated automatically.
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
from GLDrawable import GLDrawable
from OpenGL.GL import *
from BZFlag.Geometry import *
from Numeric import *
from OpenGL.GL.ARB.multitexture import *
from BZFlag.UI import GLExtension

__all__ = ['ArraySurface']


class ArraySurface(GLDrawable):
    """Drawable representing a smooth surface composed of a grid of vertices,
       contained in a Numeric array. The array may change between renderings,
       since normals are recalculated each time draw() is called. The size of
       the array may not change.

       By default this drawable will have the 'static' flag so the rendering
       engine will cache it in a display list. Disabling the static flag will
       cause normals to be recalculated at each rendering, so it will then be
       suitable for dynamic surfaces.
       """
    def __init__(self, vertices, texcoords=None):
        GLDrawable.__init__(self)
        self.vertices = vertices
        self.texcoords = texcoords
        if self.texcoords is None:
            self.texcoords = zeros(vertices.shape, vertices.typecode())

        if self.vertices.typecode() != Float32 or self.texcoords.typecode() != Float32:
            raise Exception("ArraySurface requires arrays of type Float32. " +
                            "'vertices' is of type %s, 'texcoords' is of type %s." %
                            (self.vertices.typecode(), self.texcoords.typecode()))

        self.prepareIndices()

        # Set up empty arrays of the right size for intermediate results
        (height, width) = self.vertices.shape[:2]
        self.crossProducts = zeros((height-1, width-1, 3), self.vertices.typecode())
        self.gridNormals = zeros(self.crossProducts.shape, self.vertices.typecode())
        self.normals = zeros(self.vertices.shape, self.vertices.typecode())
        self.interleaved = zeros(self.vertices.shape[:-1] + (8,), self.vertices.typecode())

        # glInterleavedArrays in PyOpenGL leaks memory, this is a workaround
        try:
            from numeric_gl import glInterleavedArrays
            self.glInterleavedArrays = glInterleavedArrays
        except ImportError:
            raise Exception("Due to a bug in PyOpenGL, you must install the numeric_gl module from " +
                            "http://py3d.org/files/numeric_gl.tar.gz")

    def prepareIndices(self):
        """Prepare an array with indices into the vertex array for triangle stripss.
           Note that these indices correspond to the vertex array only after it
           has been flattened with reshape().
           """
        self.indices = []
        (height, width) = self.vertices.shape[:2]
        for y in xrange(height - 1):
            row = []
            for x in xrange(width - 1):
                row.append(x + (y+1) * width)
                row.append(x + y * width)
            self.indices.append(array(row))

    def prepareNormals(self):
        """Prepare vertex normals for this surface, storing intermediate results
           that might be helpful for other calculations.
           """
        # Calculate cross products for all grid squares
        vx = self.vertices[1:,:-1,:] - self.vertices[:-1,:-1,:]
        vy = self.vertices[:-1,1:,:] - self.vertices[:-1,:-1,:]
        cross(vy, vx, self.crossProducts)

        # Normalize them to get normals for each grid square
        normalize(self.crossProducts, self.gridNormals)

        # Expand that to the size of the vertex array, repeating the
        # last row and column of normals.
        self.normals[:-1, :-1, :]  = self.gridNormals
        self.normals[-1, :-1, :] = self.gridNormals[-1,:,:]
        self.normals[:-1, -1, :] = self.gridNormals[:,-1,:]
        self.normals[-1,  -1, :] = self.gridNormals[-1,-1,:]

    def draw(self, rstate):
        """Calculate normals and blast our triangle strips out to OpenGL"""
        self.prepareNormals()

        self.interleaved[:,:,:2]  = self.texcoords
        self.interleaved[:,:,2:5] = self.normals
        self.interleaved[:,:,5:]  = self.vertices
        if GLExtension.multitexture:
            glClientActiveTextureARB(GL_TEXTURE0_ARB)
        self.glInterleavedArrays(GL_T2F_N3F_V3F, 0, self.interleaved)

        # We want to draw both sides of the surface. This will have OpenGL
        # automatically flip the surface normals when drawing the back side
        glDisable(GL_CULL_FACE)
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1)

        for row in self.indices:
            glDrawElementsui(GL_TRIANGLE_STRIP, row)

        glEnable(GL_CULL_FACE)
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0)


### The End ###

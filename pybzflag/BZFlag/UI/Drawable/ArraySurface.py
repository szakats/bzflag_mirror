""" BZFlag.UI.Drawable.ArraySurface

A Drawable for surfaces generated dynamically from a 2D Numeric
array of vertex coordinates. Vertex normals are generated automatically.
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
from GLDrawable import GLDrawable
from OpenGL.GL import *
from BZFlag.Geometry import *
from Numeric import *

__all__ = ['ArraySurface']

 
class ArraySurface(GLDrawable):
    """Drawable representing a smooth surface composed of a grid of vertices,
       contained in a Numeric array. The array may change between renderings,
       since normals are recalculated each time draw() is called. If the size
       of the array changes, prepareIndices() must be called.

       By default this drawable will have the 'static' flag so the rendering
       engine will cache it in a display list. Disabling the static flag will
       cause normals to be recalculated at each rendering, so it will then be
       suitable for dynamic surfaces.
       """
    def __init__(self, vertices):
        GLDrawable.__init__(self)
        self.vertices = vertices
        self.prepareIndices()

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

        # Set up empty arrays of the right size for intermediate results
        self.crossProducts = zeros((height-1, width-1, 3), self.vertices.typecode())
        self.gridNormals = zeros(self.crossProducts.shape, self.vertices.typecode())
        self.normals = zeros(self.vertices.shape, self.vertices.typecode())

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
        glVertexPointerd(reshape(self.vertices, (-1, 3)))
        glNormalPointerd(reshape(self.normals, (-1, 3)))
        glEnable(GL_VERTEX_ARRAY)
        glEnable(GL_NORMAL_ARRAY)

        # Draw the front side
        for row in self.indices:
            glDrawElementsui(GL_TRIANGLE_STRIP, row)

        # Draw the back side
        glCullFace(GL_FRONT)
        glNormalPointerd(reshape(-self.normals, (-1, 3)))
        for row in self.indices:
            glDrawElementsui(GL_TRIANGLE_STRIP, row)
        glCullFace(GL_BACK)
            
        glDisable(GL_VERTEX_ARRAY)
        glDisable(GL_NORMAL_ARRAY)

### The End ###

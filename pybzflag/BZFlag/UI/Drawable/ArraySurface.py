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
from Numeric import *

__all__ = ['ArraySurface']


def cross(u, v):
    """Perform a cross product of each vector in the given 2D array of vectors"""
    result = zeros(u.shape, u.typecode())
    ux = u[:,:,0]
    uy = u[:,:,1]
    uz = u[:,:,2]
    vx = v[:,:,0]
    vy = v[:,:,1]
    vz = v[:,:,2]
    result[:,:,0] = uz * vy - uy * vz
    result[:,:,1] = uz * vx - ux * vz
    result[:,:,2] = ux * vy - uy * vx
    return result


def magnitude(a):
    """Calculate the magnitude of each vector in the given 2D array of vectors"""
    a = a*a
    result = a[:,:,0]
    add(result, a[:,:,1], result)
    add(result, a[:,:,2], result)
    sqrt(result, result)
    return result


def normalize(a):
    """Normalize each vector in the given 2D array of vectors"""
    return a / magnitude(a)[..., NewAxis]


def calcGridNormals(vertices):
    """Calculate a face normal for each grid square in the vertex array. Returns
       an array smaller than the given array by 1 in each dimension, since the
       result corresponds to grid squares rather than grid corners.
       """
    vx = vertices[1:,:-1,:] - vertices[:-1,:-1,:]
    vy = vertices[:-1,1:,:] - vertices[:-1,:-1,:]
    return normalize(cross(vy, vx))
        
    
def calcVertexNormals(vertices):
    """Calculate vertex normals.
       The proper way to do this would be to average the face normal from each
       neighboring vertex, but just using the face normals directly looks almost
       the same and is a lot cheaper.
       """
    grid = calcGridNormals(vertices)
    result = zeros(vertices.shape, grid.typecode())
    result[:-1, :-1, :]  = grid

    # Repeat the last row and column
    result[-1, :-1, :] = grid[-1,:,:]
    result[:-1, -1, :] = grid[:,-1,:]
    result[-1,  -1, :] = grid[-1,-1,:]
    return result
 

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
        """Prepare an array with indices into the vertex array for GL_TRIANGLES,
           Note that these indices correspond to the vertex array only after it
           has been flattened with reshape().
           """
        indices = []
        (height, width) = self.vertices.shape[:2]
        for x in xrange(width - 1):
            for y in xrange(height - 1):
                # The four points in this grid square probably aren't planar, so
                # we need to split it into two triangles rather than using GL_QUADS.
                indices.append(x + y * width)
                indices.append((x+1) + y * width)
                indices.append(x + (y+1) * width)
                
                indices.append(x + (y+1) * width)
                indices.append((x+1) + y * width)
                indices.append((x+1) + (y+1) * width)
        self.indices = array(indices)

    def draw(self, rstate):
        """Calculate normals and blast our vertex array out to OpenGL"""
        glVertexPointerd(reshape(self.vertices, (-1, 3)))
        glNormalPointerd(reshape(calcVertexNormals(self.vertices), (-1, 3)))
        glEnable(GL_VERTEX_ARRAY)
        glEnable(GL_NORMAL_ARRAY)
        glDrawElementsui(GL_TRIANGLES, self.indices)
        glDisable(GL_VERTEX_ARRAY)
        glDisable(GL_NORMAL_ARRAY)

### The End ###

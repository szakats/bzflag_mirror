""" BZFlag.UI.Drawable.VRML.Mesh

Mesh is the actual drawable behind a VRML mesh. It accepts
VRML nodes parsed by Parser, and performs any additional processing
necessary to render them. This includes tesselating polygons and
calculating surface normals.
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

from BZFlag.UI.Drawable.Array import *
from OpenGL.GL import *
from Numeric import *
import os

__all__ = ('Mesh',)


class Mesh(TriangleArrayDisplayList):
    """A drawable mesh model as extracted from the VRML file.
       All relevant VRML nodes should be passed as keyword parameters,
       with their first letter lowercased.
       """
    creaseAngle = 60

    def __init__(self, **nodes):
        # Store vertices
        vertices = reshape(take(nodes['coordinate3'].value['point'],
                                self.getTriangles(nodes['indexedFaceSet'].value['coordIndex']).flat),
                           (-1, 3, 3))

        # Store texture coordinates if we have them
        if nodes.get('textureCoordinate2', None):
            texcoords = reshape(take(nodes['textureCoordinate2'].value['point'],
                                     self.getTriangles(nodes['indexedFaceSet'].value['textureCoordIndex']).flat),
                                (-1, 3, 2))
        else:
            texcoords = None

        # Store normals if we have them, otherwise calculate normals ourselves
        if nodes.get('normal', None):
            normals = reshape(take(nodes['normal'].value['vector'],
                                    self.getTriangles(nodes['indexedFaceSet'].value['normalIndex']).flat),
                               (-1, 3, 3))
        else:
            raise Exception("Automatic normal calculation is broken")

        # Now that we know how much geometry we're dealing with, initialize
        # the TriangleArray and copy our geometry into it.
        if texcoords:
            format = GL_T2F_N3F_V3F
        else:
            format = GL_N3F_V3F
        super(Mesh, self).__init__(len(vertices), format)
        self.vertices[...] = vertices.astype(Float32)
        self.normals[...] = normals.astype(Float32)
        if texcoords:
            self.texcoords[...] = texcoords.astype(Float32)

        # Store a mesh matrix if we have one
        if nodes.get('matrixTransform', None):
            self.matrix = nodes['matrixTransform'].value['matrix']
        else:
            self.matrix = None

        # Store and load a texture if we have one
        if nodes.get('texture2', None):
            children = nodes['texture2'].children
            assert(children[-1].value == 'filename')
            self.textureName = children[-2].value
            # Try to sanitize the texture name a bit. This removes surrounding quotes and
            # converts either DOS-style or UNIX-style slashes to the current OS's separator.
            if self.textureName[0] == '"' and self.textureName[-1] == '"':
                self.textureName = self.textureName[1:-1]
            self.textureName = self.textureName.replace("/", os.sep)
            self.textureName = self.textureName.replace("\\", os.sep)
            self.loadTextures()

        # Store material color if we have it
        if nodes.get('material', None):
            self.color = [1,1,1,1]
            if nodes['material'].value.has_key('diffuseColor'):
                if type(nodes['material'].value['diffuseColor'][0]) is float:
                    # Mesh has a single color value
                    self.color[:3] = nodes['material'].value['diffuseColor'][:3]
                else:
                    # Mesh has vertex colors- not implemented yet
                    pass

            if nodes['material'].value.has_key('transparency'):
                # Mesh has an alpha value
                self.color[3] = 1 - nodes['material'].value['transparency']
                self.render.blended = self.color[3] != 1
        else:
            self.color = None

    def getTriangles(self, indices):
        """Given a list of indices describing arbitrary-length polygons terminated by -1s,
           return a list of triangles, each represented as three indices.
           """
        tris = []
        polygon = []
        for index in indices:
            if index == -1:
                # -1 is the end-of-polygon marker. Tesselate any convex polygon into triangles
                for i in xrange(1, len(polygon)-1):
                    tris.append((polygon[0], polygon[i], polygon[i+1]))
                polygon = []
            else:
                # Toss another vertex on the buffer
                polygon.append(index)
        return array(tris)

##     def postprocess(self):
##         """After all the triangles have been stored, go back and calculate vertex normals"""
##         # To each triangle, add a list for accumulating vertex averages
##         for tri in self.triangles:
##             tri.normalTotal  = [tri.faceNormal] * 3

##         for vertex, uses in self.vertexMap.iteritems():
##             # Now we have a list of all the uses of this vertex, as (triangle, index) tuples.
##             # The triangle has already calculated a face normal for itself. We just have to
##             # find adjoining faces with a low enough angle between them to smooth.

##             # For every pair of uses...
##             for useIndex1 in range(len(uses)):
##                 use1 = uses[useIndex1]
##                 for useIndex2 in range(useIndex1 + 1, len(uses)):
##                     use2 = uses[useIndex2]

##                     # Compare the angle between face normals
##                     if unitVectorAngle(use1[0].faceNormal, use2[0].faceNormal) < self.creaseAngle:
##                         # These two faces are smooth enough we should combine the normals
##                         use1[0].normalTotal[use1[1]] = add(use1[0].normalTotal[use1[1]], use2[0].normals[use2[1]])
##                         use2[0].normalTotal[use2[1]] = add(use2[0].normalTotal[use2[1]], use1[0].normals[use1[1]])
##         del self.vertexMap

##         # Now go back and finish the averages by dividing
##         for tri in self.triangles:
##             for i in range(3):
##                 tri.normals[i] = normalize(tri.normalTotal[i])
##             del tri.normalTotal

    def drawToList(self, rstate):
        """Splat out our stored color, matrix, and triangles to a display list"""
        glPushMatrix()
        if self.color:
            glColor4f(*self.color)
        if self.matrix:
            glMultMatrixf(self.matrix)

        # Our normals are normalized at this point, but they might be scaled
        # by the transforms applied to each VRML mesh
        glEnable(GL_NORMALIZE)

        TriangleArrayDisplayList.drawToList(self, rstate)

        glPopMatrix()
        glColor4f(1,1,1,1)
        glDisable(GL_NORMALIZE)

### The End ###

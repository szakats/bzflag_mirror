""" BZFlag.UI.Drawable.VRML

A drawable that can load a very small subset of the VRML model format.
This module does not make any attempt to resemble a full VRML browser
implementation. It simply understands small bits of the VRML syntax
and uses this to extract geometry information.

Why VRML?

The other drawables in this directory are here because their geometry
largely depends on per-instance parameters. Objects like player models
and interface widgets, however, need little modification at runtime
and would be much more difficult to construct. Hence, the need for a
model format.

The requirements for this model format didn't leave many options open:

 - Easy to export from Blender
 - Relatively easy to parse
 - Capable of representing multiple named meshes per file, with
   transformation information for each
 - Preferably an open standard

VRML seemed to be the only format meeting all the criteria. There is
already a comprehensive VRML97 browser as part of the OpenGLContext
package, but that would be extreme overkill for our simple needs here.

This has been tested the most with VRML files produced by blender,
but it has successfully loaded files exported by other software.
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

from DisplayList import *
from OpenGL.GL import *
from BZFlag import Util
from BZFlag.Geometry import *
from Numeric import *
import re, math, BZFlag, cPickle, os, md5


lexicalScanner = re.compile(r"""
    # Keywords
    ((?P<DEF>          DEF)
    |(?P<EXTERNPROTO>  EXTERNPROTO)
    |(?P<FALSE>        FALSE)
    |(?P<IS>           IS)
    |(?P<NULL>         NULL)
    |(?P<PROTO>        PROTO)
    |(?P<ROUTE>        ROUTE)
    |(?P<TO>           TO)
    |(?P<TRUE>         TRUE)
    |(?P<USE>          USE)
    |(?P<eventIn>      eventIn)
    |(?P<eventOut>     eventOut)
    |(?P<exposedField> exposedField)
    |(?P<field>        field)

    # Data types
    |(?P<float>        \-?((\d+\.\d*|\.\d+)([eE][-+]?\d+)?|\d+[eE][-+]?\d+))
    |(?P<hex>          0[xX][\da-fA-F]*)
    |(?P<oct>          0[0-7]*)
    |(?P<dec>          \-?[1-9]\d*)

    # Terminal symbols
    |(?P<period>       \.)
    |(?P<comma>        \,)
    |(?P<openBrace>    \{)
    |(?P<closeBrace>   \})
    |(?P<openBracket>  \[)
    |(?P<closeBracket> \])

    # Identifiers
    |(?P<id>           [^\s\,\{\}\[\]]+)
    )""", re.VERBOSE | re.UNICODE)


tokenReplacements = {
    'float':   lambda value: ('number', float(value)),
    'hex':     lambda value: ('number', int(value[2:], 16)),
    'oct':     lambda value: ('number', int(value, 8)),
    'dec':     lambda value: ('number', int(value)),

    'TRUE':    lambda value: ('number', True),
    'FALSE':   lambda value: ('number', False),
    }


class VRMLParseError(Exception):
    pass


class Node:
    """A node in the VRML parser's stack"""
    def __init__(self, type, value=None, id=None, name=None, children=[]):
        self.type     = type      # Token type
        self.value    = value     # Token value
        self.id       = id        # Node id
        self.name     = name      # DEF'ed name
        self.children = children  # Nested nodes


class Reader:
    """Scans a VRML file, instantiating drawables for all readable meshes.
       The file can be specified as a file object, file name, or URI.
       After initialization, all the meshes found in the file are available
       keyed by name in the 'meshes' dictionary.
       """
    def __init__(self, name):
        self.encoding = 'utf8'
        self.meshes = {}

        self.parseStack = []
        self.namespace = {}
        f = Util.autoFile(name)
        self.parse(f)
        f.close()
        for node in self.parseStack:
            self.extractMeshes(node)
        del self.parseStack
        del self.namespace

    def parse(self, f):
        """Parse the given file object as VRML. Upon returning, there will
           be a nested tuple representation of the VRML file in parseStack
           and a dictionary of the VRML namespace in namespace.
           """
        while True:
            line = f.readline()
            if not line:
                break

            # Process the VRML header that sets the version and encoding
            if line.startswith("#VRML"):
                self.parseHeader(line)
                continue

            # Convert to Unicode using the current encoding, strip out comments.
            # Note that this makes no attempt to handle strings properly, since
            # none of the data we currently need involves strings.
            if self.encoding != 'ascii':
                line = unicode(line, self.encoding)
            line = re.sub("#.*", "", line)

            # Thanks to Python 2.2's finditer method, we can implement a fairly
            # complete lexical scanner using only a regular expression.
            for token in lexicalScanner.finditer(line):
                for tokenType, tokenValue in token.groupdict().items():
                    if tokenValue is not None:
                        self.parseToken(tokenType, tokenValue)

    def parseHeader(self, line):
        """Parse the #VRML header line. This doesn't make any attempt to validate
           the version yet, it just uses it to set the encoding.
           """
        self.encoding = re.split("\s+", line)[2]

    def parseToken(self, type, value):
        """Cheesy parser that tries to extract the information we want out of the
           lexed VRML file without having a full grammar. A full parser would
           require knowing the data type for all supported attributes, which seems
           like an awful way to design a format.
           """
        # Perform simple token replacements
        try:
            (type, value) = tokenReplacements[type](value)
        except KeyError:
            pass

        # Toss it onto the stack
        stack = self.parseStack
        stack.append(Node(type, value))

        try:
            # Combine consecutive numbers into vectors
            if stack[-1].type == 'number' and stack[-2].type == 'number':
                n = stack.pop()
                stack[-1] = Node('vector', [stack[-1].value, n.value])
            if stack[-1].type == 'number' and stack[-2].type == 'vector':
                n = stack.pop()
                stack[-1].value.append(n.value)
        except IndexError:
            pass

        try:
            # If we have a closeBracket on the top of the stack,
            # condense everything back until the last openBracket into a list.
            if stack[-1].type == 'closeBracket':
                lst = []
                # Pop values until we hit the openBracket, stuffing a list full
                # of all the numbers and vectors we find
                while True:
                    n = stack.pop()
                    if n.type == 'number' or n.type == 'vector':
                        lst.insert(0, n.value)
                    elif n.type == 'openBracket':
                        break
                stack.append(Node('list', lst))
        except IndexError:
            pass

        try:
            # After we reach the end of a section we can go back and figure out
            # what ended up getting reduced. We try to figure out attributes by
            # matching name-value pairs.
            valueTypes = ('list', 'vector', 'number')
            if stack[-1].type == 'closeBrace' and stack[-2].type in valueTypes:
                attrs = {}
                while True:
                    if stack.pop().type == 'openBrace':
                        break
                    if stack[-1].type in valueTypes and stack[-2].type == 'id':
                        n = stack.pop()
                        attrs[stack[-1].value] = n.value
                n = stack.pop()
                if n.type != 'id':
                    raise VRMLParseError("id token expected before attribute list section, found %s instead" % n.type)
                stack.append(Node('section', attrs, n.value))
        except IndexError:
            pass

        try:
            # Reduce sections ending in non-value types. Usually these are composed
            # of other sections. We'll just stick the contents into a list.
            if stack[-1].type == 'closeBrace':
                contents = []
                stack.pop()   # closeBrace
                while True:
                    n = stack.pop()
                    if n.type == 'openBrace':
                        break
                    contents.append(n)
                n = stack.pop()
                if n.type != 'id':
                    raise VRMLParseError("id token expected before section, found %s instead" % type)
                stack.append(Node('section', id=n.value, children=contents))
        except IndexError:
            pass

        try:
            # If we have (DEF id section) on the stack now, yank it all off and stow
            # the section in the VRML namespace. If we have a DEF without an id afterwards,
            # that's a parse error.
            if stack[-1].type == 'section' and stack[-3].type == 'DEF':
                section = stack.pop()   # Section contents
                n = stack.pop()         # id
                stack.pop()             # DEF
                if n.type != 'id':
                    raise VRMLParseError("id token expected after DEF, found %s instead" % n.type)
                section.name = n.value
                self.namespace[n.value] = section
        except IndexError:
            pass

        try:
            # Replace (USE id) with the object from our namespace
            if stack[-1].type == 'id' and stack[-2].type == 'USE':
                n = stack.pop()
                stack.pop()
                try:
                    pass
                    stack.append(self.namespace[n.value])
                except KeyError:
                    raise VRMLParseError("USE of name not previously DEF'ed: %s" % n.value)
        except IndexError:
            pass

    def searchUp(self, id, parents):
        """Given a parent list, search for the nearest node with the given id. Returns
           None if a suitable node can't be located.
           """
        for parent in parents:
            for sibling in parent.children:
                if sibling.id == id:
                    return sibling
        return None

    def findName(self, parents):
        """Find a node's name by searching up the node's ancestors for a non-None name"""
        for parent in parents:
            if parent.name:
                return parent.name
        return None

    def extractMeshes(self, node, parents=()):
        """Recursively traverse a tree of parsed nodes, extracting meshes into Drawables"""
        newParents = (node,) + parents

        if node.id == 'IndexedFaceSet':
            # We just found some data we can turn into a drawable. Now we just
            # need to search for the matching coordinates, material, name, and matrix.
            self.meshes[self.findName(newParents)] = Mesh(
                indexedFaceSet     = node,
                coordinate3        = self.searchUp('Coordinate3', parents),
                matrixTransform    = self.searchUp('MatrixTransform', parents),
                material           = self.searchUp('Material', parents),
                textureCoordinate2 = self.searchUp('TextureCoordinate2', parents),
                texture2           = self.searchUp('Texture2', parents),
                )

        for child in node.children:
            self.extractMeshes(child, newParents)


class Triangle:
    """Container for a triangle's vertices and normals"""
    def __init__(self, vertices, texcoords=((0,0), (0,0), (0,0))):
        self.vertices = vertices
        self.texcoords = texcoords
        self.calcFaceNormal()

        # Vertex normals default to the face normal
        self.normals = [self.faceNormal] * 3

    def calcFaceNormal(self):
        """Calculate the face normal for this triangle. If
           the vertices were named ABC, this takes the cross
           product of AB and AC, then normalizes the result.
           """
        t = self.vertices
        self.faceNormal = normalize(cross(subtract(t[1],t[0]),
                                          subtract(t[2],t[0])))


class Mesh(DisplayList):
    """A drawable mesh model as extracted from the VRML file.
       All relevant VRML nodes should be passed as keyword parameters,
       with their first letter lowercased.
       """
    creaseAngle = 60

    def set(self, **nodes):
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

        # Walk through the vertex list, storing triangles.
        # Quads will be automatically split into two triangles. We throw
        # a parse error for larger polygons, as they are used so infrequently
        # it's not worth trying to tesselate them.
        self.erase()
        polygon = []
        vertices = nodes['coordinate3'].value['point']
        for face in nodes['indexedFaceSet'].value['coordIndex']:
            if face == -1:
                # -1 is the end-of-polygon marker
                if len(polygon) == 3:
                    self.storeTriangle(polygon)
                elif len(polygon) == 4:
                    self.storeTriangle((polygon[0], polygon[1], polygon[2]))
                    self.storeTriangle((polygon[0], polygon[2], polygon[3]))
                else:
                    raise VRMLParseError("Only triangles and quads are supported. " +
                                         "Found a polygon with %s sides" % len(polygon))
                polygon = []
            else:
                # Toss another vertex on the buffer
                polygon.append(vertices[face])
        self.postprocess()

    def erase(self):
        """Clear our internal representation of the model"""
        self.triangles = []
        self.vertexMap = {}

    def storeTriangle(self, vertices):
        """Add a triangle to our internal representation, calculating normals"""
        # tupleize everything
        vertices = tuple(map(tuple, vertices))

        # The triangle will calculate its own face normal. It's up to us to
        # do vertex normals however, because it requires knowledge of the entire mesh.
        tri = Triangle(vertices)
        self.triangles.append(tri)

        # Keep a map of vertices to (triangle, index) tuples
        self.vertexMap.setdefault(vertices[0], []).append((tri, 0))
        self.vertexMap.setdefault(vertices[1], []).append((tri, 1))
        self.vertexMap.setdefault(vertices[2], []).append((tri, 2))

    def postprocess(self):
        """After all the triangles have been stored, go back and calculate vertex normals"""
        # To each triangle, add a list for accumulating vertex averages
        for tri in self.triangles:
            tri.normalTotal  = [tri.faceNormal] * 3

        for vertex, uses in self.vertexMap.iteritems():
            # Now we have a list of all the uses of this vertex, as (triangle, index) tuples.
            # The triangle has already calculated a face normal for itself. We just have to
            # find adjoining faces with a low enough angle between them to smooth.

            # For every pair of uses...
            for useIndex1 in range(len(uses)):
                use1 = uses[useIndex1]
                for useIndex2 in range(useIndex1 + 1, len(uses)):
                    use2 = uses[useIndex2]

                    # Compare the angle between face normals
                    if unitVectorAngle(use1[0].faceNormal, use2[0].faceNormal) < self.creaseAngle:
                        # These two faces are smooth enough we should combine the normals
                        use1[0].normalTotal[use1[1]] = add(use1[0].normalTotal[use1[1]], use2[0].normals[use2[1]])
                        use2[0].normalTotal[use2[1]] = add(use2[0].normalTotal[use2[1]], use1[0].normals[use1[1]])
        del self.vertexMap

        # Now go back and finish the averages by dividing
        for tri in self.triangles:
            for i in range(3):
                tri.normals[i] = normalize(tri.normalTotal[i])
            del tri.normalTotal

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

        glBegin(GL_TRIANGLES)
        for tri in self.triangles:
            glNormal3f(*tri.normals[0])
            glTexCoord2f(*tri.texcoords[0])
            glVertex3f(*tri.vertices[0])
            glNormal3f(*tri.normals[1])
            glTexCoord2f(*tri.texcoords[1])
            glVertex3f(*tri.vertices[1])
            glNormal3f(*tri.normals[2])
            glTexCoord2f(*tri.texcoords[2])
            glVertex3f(*tri.vertices[2])
        glEnd()
        glPopMatrix()
        glColor4f(1,1,1,1)
        glDisable(GL_NORMALIZE)


class Cache:
    """This is both an in-memory and on-disk cache for VRML objects."""
    def __init__(self, path=BZFlag.cachePath):
        self.objects = {}
        self.path = os.path.expanduser(path)
        try:
            os.makedirs(self.path)
        except OSError:
            # Path could already exist or it could be unreachable. We don't care.
            pass

    def getCacheFilename(self, name):
        """To uniquely identify this file, take an MD5 checksum of it"""
        f = open(name)
        hash = md5.new(f.read()).hexdigest()
        f.close
        return os.path.join(self.path, "p%s.meshDict" % hash)

    def load(self, name):
        """This checks the in-memory cache first, then the disk cache.
           Returns a dictionary associating mesh names and drawables.
           """
        try:
            # Use the filename as the memory cache key- we don't want
            # to checksum the file every time we load it from the memory cache.
            return self.objects[name]
        except KeyError:
            # Not in the memory cache, try the disk cache
            cacheFilename = self.getCacheFilename(name)
            if os.path.isfile(cacheFilename):
                f = open(cacheFilename, "rb")
                meshes = cPickle.load(f)
                f.close()
                self.objects[name] = meshes
                return meshes

            # Not in either cache, load it
            else:
                meshes = Reader(name).meshes
                self.objects[name] = meshes
                # Try to store it in the disk cache. If we can't, no big deal.
                try:
                    f = open(cacheFilename, "wb")
                    cPickle.dump(meshes, f, True)
                    f.close()
                except IOError:
                    pass
                return meshes


defaultCache = Cache()
def load(name):
    return defaultCache.load(Util.dataFile(name))

### The End ###

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

This loader has only been tested with Blender, though there's a chance
it may work with VRML files produced by other modelers.
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

from DisplayList import *
from OpenGL.GL import *
from BZFlag import Util
import re


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
    |(?P<id>           [^\s\,\.\{\}\[\]]+)    
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
            name = self.findName(newParents)
            coords = self.searchUp('Coordinate3', parents)
            material = self.searchUp('Material', parents)
            matrix = self.searchUp('MatrixTransform', parents)
            if not coords:
                raise VRMLParseError("Could not find required Coordinate3 section")

            self.meshes[name] = Mesh(faces = node,
                                     coords = coords,
                                     name = name,
                                     material = material,
                                     matrix = matrix)

        for child in node.children:
            self.extractMeshes(child, newParents)


class Mesh(DisplayList):
    """A drawable mesh model as extracted from the VRML file.
       All parameters to set() are VRML nodes.
       """
    def set(self, faces, coords, name=None, material=None, matrix=None):
        self.name = name
        self.faces = faces.value['coordIndex']
        self.vertices = coords.value['point']
        if matrix:
            self.matrix = matrix.value['matrix']
        else:
            self.matrix = None
        if material:
            self.color = material.value['diffuseColor'] + [1 - material.value['transparency']]
        else:
            self.color = None

    def drawToList(self):
        polygon = []
        glPushMatrix()
        glScalef(4,4,4)

        if self.color:
            glColor4f(*self.color)
        if self.matrix:
            glMultMatrixf(self.matrix)

        # In the VRML vertex list, a -1 indicates the end of a polygon.
        for face in self.faces:
            if face == -1:
                # Output the buffered polygon
                glBegin(GL_POLYGON)
                for vertex in polygon:
                    glVertex3f(*vertex)
                glEnd()
                polygon = []
            else:
                # Toss another vertex on the buffer
                polygon.append(self.vertices[face])
        glPopMatrix()

### The End ###

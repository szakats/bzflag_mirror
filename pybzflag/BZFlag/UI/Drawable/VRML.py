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


class Reader:
    """Scans a VRML file, instantiating drawables for all readable meshes.
       The file can be specified as a file object, file name, or URI.
       """
    def __init__(self, name):
        self.encoding = 'utf8'
        self.parseStack = []
        self.namespace = {}
        f = Util.autoFile(name)
        self.parse(f)
        f.close()
        self.extractMeshes()
        
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
        stack.append((type, value))

        try:
            # Combine consecutive numbers into vectors
            if stack[-1][0] == 'number' and stack[-2][0] == 'number':
                n = stack.pop()
                stack[-1] = ('vector', [stack[-1][1], n[1]])
            if stack[-1][0] == 'number' and stack[-2][0] == 'vector':
                n = stack.pop()
                stack[-1][1].append(n[1])
        except IndexError:
            pass

        try:
            # If we have a closeBracket on the top of the stack,
            # condense everything back until the last openBracket into a list.
            if stack[-1][0] == 'closeBracket':
                lst = []
                # Pop values until we hit the openBracket, stuffing a list full
                # of all the numbers and vectors we find
                while True:
                    (type, value) = stack.pop()
                    if type == 'number' or type == 'vector':
                        lst.insert(0, value)
                    elif type == 'openBracket':
                        break
                stack.append(('list', lst))
        except IndexError:
            pass

        try:
            # After we reach the end of a section we can go back and figure out
            # what ended up getting reduced. We try to figure out attributes by
            # matching name-value pairs.
            valueTypes = ('list', 'vector', 'number')
            if stack[-1][0] == 'closeBrace' and stack[-2][0] in valueTypes:
                attrs = {}
                while True:
                    if stack.pop()[0] == 'openBrace':
                        break
                    if stack[-1][0] in valueTypes and stack[-2][0] == 'id':
                        (type, value) = stack.pop()
                        attrs[stack[-1][1]] = value
                (type, value) = stack.pop()
                if type != 'id':
                    raise VRMLParseError("id token expected before attribute list section, found %s instead" % type)
                stack.append(('section', (value, attrs)))
        except IndexError:
            pass

        try:
            # Reduce sections ending in non-value types. Usually these are composed
            # of other sections. We'll just stick the contents into a list.
            if stack[-1][0] == 'closeBrace':
                contents = []
                stack.pop()   # closeBrace
                while True:
                    (type, value) = stack.pop()
                    if type == 'openBrace':
                        break
                    contents.append((type, value))
                (type, value) = stack.pop()
                if type != 'id':
                    raise VRMLParseError("id token expected before section, found %s instead" % type)
                stack.append(('section', (value, contents)))
        except IndexError:
            pass


        try:
            # If we have (DEF id section) on the stack now, yank it all off and stow
            # the section in the VRML namespace. If we have a DEF without an id afterwards,
            # that's a parse error.
            if stack[-1][0] == 'section' and stack[-3][0] == 'DEF':
                section = stack.pop()[1]      # Section contents
                (type, value) = stack.pop()   # id
                stack.pop()                   # DEF
                if type != 'id':
                    raise VRMLParseError("id token expected after DEF, found %s instead" % type)
                self.namespace[value] = section
        except IndexError:
            pass

        try:
            # Replace (USE id) with the object from our namespace
            if stack[-1][0] == 'id' and stack[-2][0] == 'USE':
                (type, value) = stack.pop()
                stack.pop()
                try:
                    pass
                    stack.append(self.namespace[value])
                except KeyError:
                    raise VRMLParseError("USE of name not previously DEF'ed: %s" % value)
        except IndexError:
            pass

    def extractMeshes(self):
        """After parsing, this rummages through the parse stack looking for IndexedFaceSet
           sections, and grouping the associated sections into a mesh. These meshes
           are then instantiated as drawables.
           """
        print self.parseStack
            
### The End ###

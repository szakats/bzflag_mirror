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


class Reader:
    """Scans a VRML file, instantiating drawables for all readable meshes.
       The file can be specified as a file object, file name, or URI.
       """
    def __init__(self, name):
        self.encoding = 'utf8'
        f = Util.autoFile(name)
        self.parse(f)
        f.close()

    def parse(self, f):
        """Parse the given file object as VRML"""
        while True:
            line = f.readline()
            if not line:
                break
            line = line.strip()

            # Process the VRML header that sets the version and encoding
            if line.startswith("#VRML"):
                self.parseHeader(line)
                continue

            # Convert to Unicode using the current encoding, strip out comments.
            # Note that this makes no attempt to handle strings properly, since
            # none of the data we currently need involves strings.
            line = unicode(line, self.encoding)
            line = re.sub("#.*", "", line)

            # Tokenize and resume processing the individual tokens
            for token in re.split("\s+", line):
                if token:
                    self.parseToken(token)

    def parseHeader(self, line):
        """Parse the #VRML header line. This doesn't make any attempt to validate
           the version yet, it just uses it to set the encoding.
           """
        self.encoding = re.split("\s+", line)[2]

    def parseToken(self, token):
        print token

### The End ###

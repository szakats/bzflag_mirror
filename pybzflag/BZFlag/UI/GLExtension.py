""" BZFlag.UI.GLExtension

This module supports querying OpenGL for support of various extensions,
then containing those query results in an easy to find place.
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

initialized = False
multitexture = False
textureUnits = []


def init():
    """Test for extension support"""
    global initialized, multitexture, textureUnits
    if initialized:
        return

    # Determine whether we have multitexturing
    from OpenGL.GL.ARB.multitexture import glInitMultitextureARB, GL_TEXTURE0_ARB, glActiveTextureARB
    multitexture = glInitMultitextureARB()

    # If we have multitexturing, make a list of supported texture units
    if multitexture:
        try:
            unit = GL_TEXTURE0_ARB
            while True:
                glActiveTextureARB(unit)
                textureUnits.append(unit)
                unit += 1
        except:
            pass

    initialized = True

### The End ###

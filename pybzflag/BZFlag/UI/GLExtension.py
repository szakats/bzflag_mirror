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

multitexture = False
textureUnits = []
maxAnisotropy = 1.0

def test():
    """Test for extension support."""
    global multitexture, textureUnits, maxAnisotropy
    import OpenGL.GL.ARB.multitexture
    from OpenGL.GL.EXT import texture_filter_anisotropic
    from OpenGL import GL

    # Determine whether we have multitexturing
    multitexture = OpenGL.GL.ARB.multitexture.glInitMultitextureARB()

    # If we have multitexturing, make a list of supported texture units
    if multitexture:
        try:
            unit = OpenGL.GL.ARB.multitexture.GL_TEXTURE0_ARB
            while True:
                OpenGL.GL.ARB.multitexture.glActiveTextureARB(unit)
                textureUnits.append(unit)
                unit += 1
        except:
            pass

    # Determine whether we have anisotropic filtering
    try:
        maxAnisotropy = GL.glGetFloatv(texture_filter_anisotropic.GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT)
    except:
        maxAnisotropy = 1.0

### The End ###

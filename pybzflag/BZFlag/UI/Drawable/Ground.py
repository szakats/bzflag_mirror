""" BZFlag.UI.Drawable.Ground

A class to draw the ground in the world
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
from OpenGL.GL.ARB.multitexture import *
from BZFlag.UI import GLExtension


class Ground(DisplayList):
    textureNames = ('grass_base.png', 'grass_overlay.jpeg')

    def set(self, size):
        self.size = size / 2
        self.render.textures[1].texEnv = GL_MODULATE

    def drawToList(self):
        glPushMatrix()
        glDisable(GL_CULL_FACE)
        glBegin(GL_QUADS)
        glNormal3f(0, 0, 1);

        baseTexRepeats = 30
        overlayTexRepeats = 1
        glTexCoord2f(baseTexRepeats, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, overlayTexRepeats, 0)
        glVertex3f(self.size, -self.size, 0)
        glTexCoord2f(0,0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0,0)
        glVertex3f(-self.size, -self.size, 0)
        glTexCoord2f(0, baseTexRepeats)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, overlayTexRepeats)
        glVertex3f(-self.size, self.size, 0)
        glTexCoord2f(baseTexRepeats, baseTexRepeats)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, overlayTexRepeats, overlayTexRepeats)
        glVertex3f(self.size, self.size, 0)

        glEnd()
        glEnable(GL_CULL_FACE)
        glPopMatrix()

### The End ###

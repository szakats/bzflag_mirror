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

class Ground(DisplayList):
    textureName = 'ground.png'
    def drawToList(self, size):
        self.size = size / 2
        glPushMatrix()
        glDisable(GL_CULL_FACE)
        glBegin(GL_QUADS)
        texRepeats = 60
        glNormal3f(0, 0, 1);
        glTexCoord2f(0.0, 0.0)
        glVertex3f(self.size, -self.size, 0)
        glTexCoord2f(texRepeats, 0.0)
        glVertex3f(-self.size, -self.size, 0)
        glTexCoord2f(texRepeats, texRepeats)
        glVertex3f(-self.size, self.size, 0)
        glTexCoord2f(0.0, texRepeats)
        glVertex3f(self.size, self.size, 0)
        glEnd()
        glEnable(GL_CULL_FACE)
        glPopMatrix()


### The End ###

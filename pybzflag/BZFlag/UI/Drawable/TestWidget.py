""" BZFlag.UI.Drawable.Wall

A class to draw the walls in the world
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

class TestWidget(DisplayList):
    def set(self, center):
        self.center = center

    def drawToList(self):
        size = 0.2
        glPushMatrix()
	glLoadIdentity()
        glColor3f(1.0, 0.0, 0.0)
        # lower left corner
	glTranslatef(*self.center)
	glBegin(GL_QUADS)
        # front
	glVertex3f(-size, size, -size)
	glVertex3f(size, size, -size)
	glVertex3f(size, -size, -size)
	glVertex3f(-size, -size, -size)
	# back
	glVertex3f(size, size, size)
	glVertex3f(-size, size, size)
	glVertex3f(-size, -size, size)
	glVertex3f(size, -size, size)
        # top
	glVertex3f(-size, size, -size)
	glVertex3f(-size, size, size)
	glVertex3f(size, size, size)
	glVertex3f(size, size, -size)
        # bottom
	glVertex3f(-size, -size, -size)
	glVertex3f(size, -size, -size)
	glVertex3f(size, -size, size)
	glVertex3f(-size, -size, size)
        # left
	glVertex3f(-size, -size, size)
	glVertex3f(-size, size, size)
	glVertex3f(-size, size, -size)
	glVertex3f(-size, -size, -size)
        # right
	glVertex3f(size, -size, -size)
	glVertex3f(size, size, -size)
	glVertex3f(size, size, size)
	glVertex3f(size, -size, size)
	glEnd()
	glColor3f(1.0, 1.0, 1.0)
	glPopMatrix()

### The End ###

""" BZFlag.UI.Drawable.Base

Class for drawing the team bases in the world
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


class BaseTops(DisplayList):
    def set(self, team, center, angle, size):
        self.team = team
        self.center = center
        self.angle = angle
        self.size = size

    def drawToList(self, rstate):
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        if self.team == 'red':
            glColor3f(1.0, 0.0, 0.0)
        if self.team == 'green':
            glColor3f(0.0, 1.0, 0.0)
        if self.team == 'blue':
            glColor3f(0.0, 0.0, 1.0)
        if self.team == 'purple':
            glColor3f(1.0, 0.0, 1.0)
        glBegin(GL_QUADS)
        # Z+
        glNormal3f(0, 0, 1);
        glVertex3f(self.size[0], -self.size[1], self.size[2]);
        glVertex3f(self.size[0], self.size[1], self.size[2]);
        glVertex3f(-self.size[0], self.size[1], self.size[2]);
        glVertex3f(-self.size[0], -self.size[1], self.size[2]);
        # Z-
        glNormal3f(0, 0, -1);
        glVertex3f(-self.size[0], -self.size[1], 0);
        glVertex3f(-self.size[0], self.size[1], 0);
        glVertex3f(self.size[0], self.size[1], 0);
        glVertex3f(self.size[0], -self.size[1], 0);
        glEnd()
        glColor3f(1.0, 1.0, 1.0)
        glPopMatrix()


### The End ###

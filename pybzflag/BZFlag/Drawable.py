""" BZFlag.Drawable

OpenGL drawing definitions for various world objects
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
from OpenGL.GL import *
from BZFlag.UI.Texture import Texture

def cross(a, b):
  return (a[2] * b[1] - a[1] * b[2], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])

class GLDrawable:
  def __init__(self):
    self.list = glGenLists(1)
    self.texture = None

  def __del__(self):
    glDeleteLists(self.list, 1)

  def draw(self):
    pass

  def drawWithName(self, name):
    pass

class Ground(GLDrawable):
  def __init__(self, size):
    GLDrawable.__init__(self)
    self.size = size / 2
    self.texture = Texture('data/ground.png')
    glNewList(self.list, GL_COMPILE)
    glPushMatrix()
    glDisable(GL_CULL_FACE)
    glBegin(GL_QUADS)
    glTexCoord2f(0.0, 0.0)
    glVertex3f(self.size, -self.size, 0)
    glTexCoord2f(1.0, 0.0)
    glVertex3f(-self.size, -self.size, 0)
    glTexCoord2f(1.0, 1.0)
    glVertex3f(-self.size, self.size, 0)
    glTexCoord2f(0.0, 1.0)
    glVertex3f(self.size, self.size, 0)
    glEnd()
    glEnable(GL_CULL_FACE)
    glPopMatrix()
    glEndList()

  def draw(self):
    glCallList(self.list)

class BaseTops(GLDrawable):
  def __init__(self, team, center, angle, size):
    GLDrawable.__init__(self)
    self.team = team
    self.center = center
    self.angle = angle * 180 / 3.1415926
    self.size = size
    glNewList(self.list, GL_COMPILE)
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
      glEndList()

  def draw(self):
    glCallList(self.list)

class Wall(GLDrawable):
  def __init__(self, center, angle, size):
    GLDrawable.__init__(self)
    self.center = center
    self.angle = angle * 180 / 3.1415926
    self.size = size
    self.texture = Texture('data/wall.png')
    glNewList(self.list, GL_COMPILE)
    glPushMatrix()
    glTranslatef(*self.center)
    glRotatef(self.angle, 0.0, 0.0, 1.0)
    glDisable(GL_CULL_FACE)
    glBegin(GL_TRIANGLE_STRIP)
    glTexCoord2f(0, 0)
    glVertex3f(0, -self.size[1], 0)
    glTexCoord2f(0, 1)
    glVertex3f(0, -self.size[1], self.size[2])
    glTexCoord2f((self.size[1] * 2) / self.size[2], 0)
    glVertex3f(0, self.size[1], 0)
    glTexCoord2f((self.size[1] * 2) / self.size[2], 1)
    glVertex3f(0, self.size[1], self.size[2])
    glEnd()
    glEnable(GL_CULL_FACE)
    glPopMatrix()
    glEndList()

  def draw(self):
    glCallList(self.list)

class BoxSides(GLDrawable):
  def __init__(self, center, angle, size):
    GLDrawable.__init__(self)
    self.center = center
    self.angle = angle * 180 / 3.1415926
    self.size = size
    self.texture = Texture('data/boxwall.png')
    glNewList(self.list, GL_COMPILE)
    glPushMatrix()
    glTranslatef(*self.center)
    glRotatef(self.angle, 0.0, 0.0, 1.0)
    glBegin(GL_QUADS)
    # Y+ side
    glNormal3f(0, 1, 0)
    glTexCoord2f(0, 0)
    glVertex3f(-self.size[0], self.size[1], 0)
    glTexCoord2f(0, self.size[2] / 4)
    glVertex3f(-self.size[0], self.size[1], self.size[2])
    glTexCoord2f(self.size[0] / 2, self.size[2] / 4)
    glVertex3f(self.size[0], self.size[1], self.size[2])
    glTexCoord2f(self.size[0] / 2, 0)
    glVertex3f(self.size[0], self.size[1], 0)
    # Y- side
    glNormal3f(0, -1, 0)
    glTexCoord2f(0, 0)
    glVertex3f(self.size[0], -self.size[1], 0)
    glTexCoord2f(0, self.size[2] / 4)
    glVertex3f(self.size[0], -self.size[1], self.size[2])
    glTexCoord2f(self.size[0] / 2, self.size[2] / 4)
    glVertex3f(-self.size[0], -self.size[1], self.size[2])
    glTexCoord2f(self.size[0] / 2, 0)
    glVertex3f(-self.size[0], -self.size[1], 0)
    # X+ side
    glNormal3f(1, 0, 0)
    glTexCoord2f(0, 0)
    glVertex3f(self.size[0], self.size[1], 0)
    glTexCoord2f(0, self.size[2] / 4)
    glVertex3f(self.size[0], self.size[1], self.size[2])
    glTexCoord2f(self.size[1] / 2, self.size[2] / 4)
    glVertex3f(self.size[0], -self.size[1], self.size[2])
    glTexCoord2f(self.size[1] / 2, 0)
    glVertex3f(self.size[0], -self.size[1], 0)
    # X- side
    glNormal3f(-1, 0, 0)
    glTexCoord2f(0, 0)
    glVertex3f(-self.size[0], -self.size[1], 0)
    glTexCoord2f(0, self.size[2] / 4)
    glVertex3f(-self.size[0], -self.size[1], self.size[2])
    glTexCoord2f(self.size[1] / 2, self.size[2] / 4)
    glVertex3f(-self.size[0], self.size[1], self.size[2])
    glTexCoord2f(self.size[1] / 2, 0)
    glVertex3f(-self.size[0], self.size[1], 0)
    glEnd()
    glPopMatrix()
    glEndList()

  def draw(self):
    glCallList(self.list)

class BoxTops(GLDrawable):
  def __init__(self, center, angle, size):
    GLDrawable.__init__(self)
    self.center = center
    self.angle = angle * 180 / 3.1415926
    self.size = size
    self.texture = Texture('data/boxtops.png')
    glNewList(self.list, GL_COMPILE)
    glPushMatrix()
    glTranslatef(*self.center)
    glRotatef(self.angle, 0.0, 0.0, 1.0)
    glBegin(GL_QUADS)
    # Z+ side
    glNormal3f(0, 0, 1)
    glTexCoord2f(self.size[0] / 2, 0)
    glVertex3f(self.size[0], -self.size[1], self.size[2])
    glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
    glVertex3f(self.size[0], self.size[1], self.size[2])
    glTexCoord2f(0, self.size[1] / 2)
    glVertex3f(-self.size[0], self.size[1], self.size[2])
    glTexCoord2f(0, 0)
    glVertex3f(-self.size[0], -self.size[1], self.size[2])
    # Z- side
    glNormal3f(0, 0, -1)
    glTexCoord2f(0, 0)
    glVertex3f(-self.size[0], -self.size[1], 0)
    glTexCoord2f(0, self.size[1] / 2)
    glVertex3f(-self.size[0], self.size[1], 0)
    glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
    glVertex3f(self.size[0], self.size[1], 0)
    glTexCoord2f(self.size[0] / 2, 0)
    glVertex3f(self.size[0], -self.size[1], 0)
    glEnd()
    glPopMatrix()
    glEndList()

  def draw(self):
    glCallList(self.list)

class Pyramid(GLDrawable):
  def __init__(self, center, angle, size, flip):
    # FIXME - respect flipz
    GLDrawable.__init__(self)
    self.center = center
    self.angle = angle * 180 / 3.1415926
    self.size = size;
    self.flip = flip;
    self.texture = Texture('data/pyrwall.png')
    glNewList(self.list, GL_COMPILE)
    glPushMatrix()
    glTranslatef(*self.center)
    glRotatef(self.angle, 0.0, 0.0, 1.0)
    glBegin(GL_QUADS)
    # Z- side
    glNormal3f(0, 0, -1)
    glTexCoord2f(0, 0)
    glVertex3f(-self.size[0], -self.size[1], 0)
    glTexCoord2f(0, self.size[1] / 2)
    glVertex3f(-self.size[0], self.size[1], 0)
    glTexCoord2f(self.size[0] / 2, self.size[1] / 2)
    glVertex3f(self.size[0], self.size[1], 0)
    glTexCoord2f(self.size[0] / 2, 0)
    glVertex3f(self.size[0], -self.size[1], 0)
    glEnd()
    glBegin(GL_TRIANGLES)
    # X+ side
    norm = cross((self.size[0], -self.size[1], self.size[2]), (self.size[0] * 2, 0, 0))
    glNormal3f(*norm)
    glTexCoord2f(0, 0)
    glVertex3f(-self.size[0], self.size[1], 0)
    glTexCoord2f(self.size[0] / 4, self.size[2] / 4)
    glVertex3f(0, 0, self.size[2])
    glTexCoord2f(self.size[0] / 2, 0)
    glVertex3f(self.size[0], self.size[1], 0)
    # Y+ side
    norm = cross((self.size[0], -self.size[1], self.size[2]), (0, self.size[1] * 2, 0))
    glNormal3f(*norm)
    glTexCoord2f(0, 0)
    glVertex3f(self.size[0], self.size[1], 0)
    glTexCoord2f(self.size[1] / 4, self.size[2] / 4)
    glVertex3f(0, 0, self.size[2])
    glTexCoord2f(self.size[1] / 2, 0)
    glVertex3f(self.size[0], -self.size[1], 0)
    # X- side
    norm = cross((-self.size[0], self.size[1], self.size[2]), (-self.size[0] * 2, 0, 0))
    glNormal3f(*norm)
    glTexCoord2f(0, 0)
    glVertex3f(self.size[0], -self.size[1], 0)
    glTexCoord2f(self.size[0] / 4, self.size[2] / 4)
    glVertex3f(0, 0, self.size[2])
    glTexCoord2f(self.size[0] / 2, 0)
    glVertex3f(-self.size[0], -self.size[1], 0)
    # Y- side
    norm = cross((-self.size[0], self.size[1], self.size[2]), (0, self.size[1] * 2, 0))
    glNormal3f(*norm)
    glTexCoord2f(0, 0)
    glVertex3f(-self.size[0], -self.size[1], 0)
    glTexCoord2f(self.size[1] / 4, self.size[2] / 4)
    glVertex3f(0, 0, self.size[2])
    glTexCoord2f(self.size[1] / 2, 0)
    glVertex3f(-self.size[0], self.size[1], 0)
    glEnd()
    glPopMatrix()
    glEndList()

  def draw(self):
    glCallList(self.list)

class TeleporterField(GLDrawable):
  def __init__(self, center, angle, size):
    GLDrawable.__init__(self)
    self.center = center
    self.angle = angle * 180 / 3.1415926
    self.size = size
    glNewList(self.list, GL_COMPILE)
    glPushMatrix()
    glTranslatef(*self.center)
    glRotatef(self.angle, 0.0, 0.0, 1.0)
    glColor3f(0.1, 0.1, 0.1)
    glBegin(GL_QUADS)
    # X+ side
    glNormal3f(1, 0, 0)
    glVertex3f(self.size[0] / 2, self.size[1], 0)
    glVertex3f(self.size[0] / 2, self.size[1], self.size[2])
    glVertex3f(self.size[0] / 2, -self.size[1], self.size[2])
    glVertex3f(self.size[0] / 2, -self.size[1], 0)
    # X- side
    glNormal3f(-1, 0, 0)
    glVertex3f(-self.size[0] / 2, -self.size[1], 0)
    glVertex3f(-self.size[0] / 2, -self.size[1], self.size[2])
    glVertex3f(-self.size[0] / 2, self.size[1], self.size[2])
    glVertex3f(-self.size[0] / 2, self.size[1], 0)
    glEnd()
    glColor3f(1.0, 1.0, 1.0)
    glPopMatrix()
    glEndList()

  def draw(self):
    glCallList(self.list)

class TeleporterBorder(GLDrawable):
  def __init__(self, center, angle, size, border):
    GLDrawable.__init__(self)
    self.center = center
    self.angle = angle * 180 / 3.1415926
    self.size = size
    self.border = border
    self.texture = Texture('data/caution.png')
    glNewList(self.list, GL_COMPILE)
    glPushMatrix()
    glTranslatef(*self.center)
    glRotatef(self.angle, 0.0, 0.0, 1.0)
    glBegin(GL_TRIANGLE_STRIP)
    glNormal3f(-1, 0, 0)
    glTexCoord2f(0, 0)
    glVertex3f(-self.border / 2, self.size[1] + self.border, 0)
    glTexCoord2f(self.border, 0)
    glVertex3f(-self.border / 2, self.size[1], 0)
    glTexCoord2f(0, self.size[2] + self.border)
    glVertex3f(-self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
    glTexCoord2f(self.border, self.size[2])
    glVertex3f(-self.border / 2, self.size[1], self.size[2])
    glTexCoord2f(2 * self.border + 2 * self.size[1], self.size[2] + self.border)
    glVertex3f(-self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
    glTexCoord2f(self.border + 2 * self.size[1], self.size[2])
    glVertex3f(-self.border / 2, -self.size[1], self.size[2])
    glTexCoord2f(2 * self.border + 2 * self.size[1], 0)
    glVertex3f(-self.border / 2, -self.size[1] - self.border, 0)
    glTexCoord2f(self.border + 2 * self.size[1], 0)
    glVertex3f(-self.border / 2, -self.size[1], 0)
    glEnd()
    glFrontFace(GL_CW)
    glBegin(GL_TRIANGLE_STRIP)
    glNormal3f(1, 0, 0)
    glTexCoord2f(0, 0)
    glVertex3f(self.border / 2, self.size[1] + self.border, 0)
    glTexCoord2f(self.border, 0)
    glVertex3f(self.border / 2, self.size[1], 0)
    glTexCoord2f(0, self.size[2] + border)
    glVertex3f(self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
    glTexCoord2f(self.border, self.size[2])
    glVertex3f(self.border / 2, self.size[1], self.size[2])
    glTexCoord2f(2 * self.border + 2 * self.size[1], self.size[2] + self.border)
    glVertex3f(self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
    glTexCoord2f(self.border + 2 * self.size[1], self.size[2])
    glVertex3f(self.border / 2, -self.size[1], self.size[2])
    glTexCoord2f(2 * self.border + 2 * self.size[1], 0)
    glVertex3f(self.border / 2, -self.size[1] - self.border, 0)
    glTexCoord2f(self.border + 2 * self.size[1], 0)
    glVertex3f(self.border / 2, -self.size[1], 0)
    glEnd()
    glFrontFace(GL_CCW)
    glBegin(GL_QUADS)
    # top
    glNormal3f(0.4, 0, 1)
    glTexCoord2f(0.4, 0.0001)
    glVertex3f(self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
    glTexCoord2f(2 * self.border + 2 * self.size[1] + 0.4, 0.0001)
    glVertex3f(self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
    glTexCoord2f(2 * self.border + 2 * self.size[1] + 0.4, 0)
    glVertex3f(-self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
    glTexCoord2f(0.4, 0)
    glVertex3f(-self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
    # underside of top
    glNormal3f(0, 0, -1)
    glTexCoord2f(0.4, 0)
    glVertex3f(-self.border / 2, -self.size[1] - self.border, self.size[2])
    glTexCoord2f(2 * self.border + 2 * self.size[1] + 0.4, 0)
    glVertex3f(-self.border / 2, self.size[1] + self.border, self.size[2])
    glTexCoord2f(2 * self.border + 2 * self.size[1] + 0.4, 0.0001)
    glVertex3f(self.border / 2, self.size[1] + self.border, self.size[2])
    glTexCoord2f(0.4, 0.0001)
    glVertex3f(self.border / 2, -self.size[1] - self.border, self.size[2])
    # Y+ outside
    glNormal3f(0, 1, 0)
    glTexCoord2f(self.border, 0)
    glVertex3f(-self.border / 2, self.size[1] + border, 0)
    glTexCoord2f(self.border, self.size[2] + border)
    glVertex3f(-self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
    glTexCoord2f(0, self.size[2] + self.border)
    glVertex3f(self.border / 2, self.size[1] + self.border, self.size[2] + self.border)
    glTexCoord2f(0, 0)
    glVertex3f(self.border / 2, self.size[1] + self.border, 0)
    # Y+ inside
    glNormal3f(0, -1, 0)
    glTexCoord2f(0, 0)
    glVertex3f(self.border / 2, self.size[1], 0)
    glTexCoord2f(0, self.size[2])
    glVertex3f(self.border / 2, self.size[1], self.size[2])
    glTexCoord2f(self.border, self.size[2])
    glVertex3f(-self.border / 2, self.size[1], self.size[2])
    glTexCoord2f(self.border, 0)
    glVertex3f(-self.border / 2, self.size[1], 0);
    # Y- outside
    glNormal3f(0, 1, 0)
    glTexCoord2f(0, 0)
    glVertex3f(self.border / 2, -self.size[1] - self.border, 0)
    glTexCoord2f(0, self.size[2] + self.border)
    glVertex3f(self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
    glTexCoord2f(self.border, self.size[2] + self.border)
    glVertex3f(-self.border / 2, -self.size[1] - self.border, self.size[2] + self.border)
    glTexCoord2f(self.border, 0)
    glVertex3f(-self.border / 2, -self.size[1] - self.border, 0)
    # Y- inside
    glNormal3f(0, -1, 0)
    glTexCoord2f(self.border, 0)
    glVertex3f(-self.border / 2, -self.size[1], 0)
    glTexCoord2f(self.border, self.size[2])
    glVertex3f(-self.border / 2, -self.size[1], self.size[2])
    glTexCoord2f(0, self.size[2])
    glVertex3f(self.border / 2, -self.size[1], self.size[2])
    glTexCoord2f(0, 0)
    glVertex3f(self.border / 2, -self.size[1], 0)
    # Y+ leg bottom
    glNormal3f(0, 0, -1)
    glTexCoord2f(self.border, self.border)
    glVertex3f(self.border / 2, self.size[1] + self.border, 0)
    glTexCoord2f(0, self.border)
    glVertex3f(self.border / 2, self.size[1], 0)
    glTexCoord2f(0, 0)
    glVertex3f(-self.border / 2, self.size[1], 0)
    glTexCoord2f(self.border, 0)
    glVertex3f(-self.border / 2, self.size[1] + self.border, 0)
    # Y- leg bottom
    glNormal3f(0, 0, -1);
    glTexCoord2f(self.border, self.border);
    glVertex3f(-self.border / 2, -self.size[1] - self.border, 0);
    glTexCoord2f(self.border, 0);
    glVertex3f(-self.border / 2, -self.size[1], 0);
    glTexCoord2f(0, 0);
    glVertex3f(self.border / 2, -self.size[1], 0);
    glTexCoord2f(0, self.border);
    glVertex3f(self.border / 2, -self.size[1] - self.border, 0);
    glEnd()
    glPopMatrix()
    glEndList()

  def draw(self):
    glCallList(self.list)

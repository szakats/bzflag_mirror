""" BZFlag.UI.Objects.Ground

Rendering for the ground
"""
#
# Python BZFlag Protocol Package
# Copyright (C) 2003 David Trowbridge <davidtrowbridge@users.sourceforge.net>
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
import Image

class Ground:
    def __init__(self):
        image = Image.open("ground.png")
	ix = image.size[0]
	iy = image.size[1]
#	image = image.tostring("raw", "RGBA", 0, -1)
#	self.texture = glGenTextures(1)
#	glBindTexture(GL_TEXTURE_2D, self.texture)
#	glPixelStorei(GL_UNPACK_ALIGNMENT,1)
#	glTexImage2D(GL_TEXTURE_2D, 0, 3, ix, iy, 0, GL_RGBA, GL_UNSIGNED_BYTE, image)
#	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
#	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
#	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
#	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)

	self.list = glGenLists(1)
	glNewList(self.list, GL_COMPILE)
	glDisable(GL_CULL_FACE)
	textureScale = 1.0
	wallSize = 400.0

	glBegin(GL_QUADS)
	glNormal3f(0, 0, 1)
	glTexCoord2f(0.0, 0.0)
	glVertex3f(wallSize, -wallSize, 0)
	glTexCoord2f(textureScale, 0.0)
	glVertex3f(-wallSize, -wallSize, 0)
	glTexCoord2f(textureScale, textureScale)
	glVertex3f(-wallSize, wallSize, 0)
	glTexCoord2f(0.0, textureScale)
	glVertex3f(wallSize, wallSize, 0)
	glEnd()

	glEnable(GL_CULL_FACE)
	glEndList()

    def draw(self):
        glColor3f(1, 1, 1)
	glEnable(GL_COLOR_MATERIAL)
#	glBindTexture(self.texture)
	glCallList(self.list)
	glColor3f(1, 1, 1)

""" BZFlag.UI.Drawable.Base

Class for drawing the team bases in the world
"""
#
# Python BZFlag Package
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
import Box


colorScheme = {
    'red':    (1, 0.5, 0.5),
    'green':  (0.7, 1, 0.7),
    'blue':   (0.7, 0.7, 1),
    'purple': (1, 0.5, 0.8),
    }


class BaseDecal(Box.TopDecal):
    textureName = "basedecal%02d.png:40"
    def set(self, base):
        Box.TopDecal.set(self, base)
        self.color = colorScheme[base.team]

    def drawToList(self, rstate):
        glDisable(GL_LIGHTING)
        glColor3f(*self.color)
        Box.TopDecal.drawToList(self, rstate)
        glEnable(GL_LIGHTING)


def detectBaseDrawables(base):
    """Given a base WorldObject, return a list of the drawables that should be used
       to represent it. This starts with a box, then adds some decorations to make it a base.
       """
    drawables = Box.detectBoxDrawables(base)
    drawables.append(BaseDecal(base))
    return drawables

### The End ###

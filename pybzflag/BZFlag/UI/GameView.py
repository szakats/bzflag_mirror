""" BZFlag.UI.GameView

A frontend for running several views at once to provide
the full in-game experience.
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

from BZFlag.UI import Viewport, ThreeDView, RadarView
from OpenGL.GL import *


class HUDBorderView:
    """A view that draws a shaded background and thin border"""
    def __init__(self, viewport):
        viewport.fov = None
        viewport.onDrawFrame.observe(self.render)

    def render(self):
        glEnable(GL_BLEND)
        glDisable(GL_LIGHTING)
        glDisable(GL_CULL_FACE)
        glDisable(GL_COLOR_MATERIAL)
        glDisable(GL_DEPTH_TEST)
        glLoadIdentity()

        glColor4f(1,1,1,1)
        glBegin(GL_LINE_LOOP)
        glVertex2f(0,0)
        glVertex2f(1,0)
        glVertex2f(1,1)
        glVertex2f(0,1)
        glEnd()

        glColor4f(0,0,0,0.2)
        glBegin(GL_POLYGON)
        glVertex2f(0,0)
        glVertex2f(1,0)
        glVertex2f(1,1)
        glVertex2f(0,1)
        glEnd()


def attach(game, eventLoop):
    viewport = Viewport.OpenGLViewport(eventLoop, (800,600))

    view3d   = ThreeDView.ThreeDView(game, viewport)
    ThreeDView.ThreeDController(view3d, viewport)

    hud = viewport.region((50,50,600,200))
    HUDBorderView(hud)

    RadarView.RadarView(game, hud.region((10,10,180,180)))

### The End ###

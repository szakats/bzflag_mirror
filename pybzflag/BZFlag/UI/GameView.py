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

from BZFlag.UI import Viewport, ThreeDView, RadarView, Layout
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
        glDisable(GL_TEXTURE_2D)
        glLoadIdentity()

        # Draw slightly inside the viewport, so our border doesn't get half-chopped-off
        glScalef(0.995, 0.995, 0.995)

        def square():
            glVertex2f(-1,-1)
            glVertex2f( 1,-1)
            glVertex2f( 1, 1)
            glVertex2f(-1, 1)

        # Creamy translucent filling
        glColor4f(0,0,0,0.3)
        glBegin(GL_POLYGON)
        square()
        glEnd()

        # Bright border
        glColor4f(1,1,1,1)
        glBegin(GL_LINE_LOOP)
        square()
        glEnd()


def attach(game, eventLoop):
    viewport = Viewport.OpenGLViewport(eventLoop, (800,600))

    view3d   = ThreeDView.ThreeDView(game, viewport)
    ThreeDView.ThreeDController(view3d, viewport)

    hud = viewport.region(Layout.Rect(viewport).margin(0.01).bottom(0.3))
    HUDBorderView(hud)

    RadarView.RadarView(game, hud.region(Layout.Rect(hud).left(100)))

    return viewport

### The End ###

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

from BZFlag.UI import Viewport, ThreeDView, ThreeDControl, RadarView, Layout, HUD
import BZFlag
from BZFlag import Animated


def attach(game, eventLoop):
    viewport = Viewport.OpenGLViewport(eventLoop, (800,600))
    time = Animated.Timekeeper()
    padding = 10
    fullHudSize = 0.3

    # 3D view in the root viewport
    view3d   = ThreeDView.ThreeDView(game, viewport)
    ThreeDControl.Viewing(view3d, viewport)
    remaining = Layout.Rect(viewport).margin(padding)

    # HUD panel along the bottom of the screen, with animated resize
    hudSize = Animated.Value(Animated.SigmoidApproach(fullHudSize, 15, (0, fullHudSize)), fullHudSize)
    def onSetupFrame():
        hudSize.integrate(time.step())
    def onKeyDown(event):
        if event.unicode == " ":
            if hudSize.f.target == fullHudSize:
                hudSize.f.target = 0
            else:
                hudSize.f.target = fullHudSize
    viewport.onSetupFrame.observe(onSetupFrame)
    viewport.onKeyDown.observe(onKeyDown)
    (remaining, hudRect) = remaining.hSplit(lambda r: (1-hudSize.value) * r[3])

    hud = viewport.region(hudRect)
    HUD.Panel(hud)

    # Stick a radar in the left side of the HUD panel. The lambda expression
    # lazily evaluates the HUD panel's height and uses it as a width for the left
    # split, keeping the radar square.
    RadarView.RadarView(game, hud.region(Layout.Rect(hud).left(lambda r: r[3]).margin(0.03)))

    # Another HUD panel, just for illustrative purposes
    (remaining, fooRect) = remaining.margin(0,0,padding,0).vSplit(0.9)
    HUD.Panel(viewport.region(fooRect))

    # Some text in the remaining area
    HUD.Text(viewport.region(remaining), BZFlag.name)

    return viewport

### The End ###

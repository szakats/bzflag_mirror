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
import BZFlag, sys
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

    hudPanel = viewport.region(hudRect)
    HUD.Panel(hudPanel)

    # Stick a radar in the left side of the HUD panel. The lambda expression
    # lazily evaluates the panel's height and uses it as a width for the left
    # split, keeping the radar square. The rest of the panel gets used for messages.
    (radarRect, messageRect) = Layout.Rect(hudPanel).vSplit(lambda r: r[3])
    RadarView.RadarView(game, hudPanel.region(radarRect.margin(2)))

    # Redirect our stdout to this text panel
    sys.stdout = HUD.ScrolledText(hudPanel.region(messageRect.margin(2)))
    print BZFlag.about

    # Logo-thingy!
    HUD.Text(viewport.region(remaining), BZFlag.name,
             shadow=True, color=(1,1,0), fontSize=35)
    return viewport

### The End ###

""" BZFlag.UI.StereoView

A 3d scene renderer that does side-by-side 2-channel stereo (usually for SGI
visualization hardware)
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

from BZFlag.UI import Viewport, ThreeDView, ThreeDControl, ThreeDRender, Layout, HUD
import BZFlag, math
from OpenGL.GL import *


class RightCamera(ThreeDRender.Camera):
    """The left eye has our 'master camera' that is operated by the ThreeDControl.Viewing
       instance. This class replaces the camera for the right eye, and manages setting the
       azimuthOffset correctly when switching between views.
       """
    def __init__(self, left, eyesep=12.5):
        self.left = left
        self.eyesep = eyesep

    def load(self):
        angle = math.atan(self.eyesep / self.left.distance)
        self.left.azimuthOffset = angle
        self.left.load()
        self.left.azimuthOffset = -angle

        
class StereoView:
    """Shows a side-by-side stereo view of the BZFlag game, renderable
       to an OpenGLViewport.
       """
    def __init__(self, game, viewport):
        self.lefteye  = ThreeDView.ThreeDView(game, viewport.region(Layout.Rect(viewport).left(0.5)))
        self.righteye = ThreeDView.ThreeDView(game, viewport.region(Layout.Rect(viewport).right(0.5)))
        ThreeDControl.Viewing(self.lefteye, viewport)
        self.righteye.camera = RightCamera(self.lefteye.camera, 30)


def attach(game, eventLoop):
    viewport = Viewport.OpenGLViewport(eventLoop, (1600, 600))
    view = StereoView(game, viewport)
    return viewport

""" BZFlag.UI.AnaglyphView

A 3d scene renderer that does anaglyph stereo
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

import pygame, math, BZFlag
from pygame.locals import *
from BZFlag.UI.ThreeDView import *
from OpenGL.GL import *
from OpenGL.GLU import *
import sys

class AnaglyphView(ThreeDView):
    """Shows an anaglyph-stereo 3d view of the BZFlag game, renderable to an
       OpenGLViewport. Probably not too useful, as anaglyph gives people headaches fast
       """
    def __init__(self, game, viewport):
        ThreeDView.__init__(self, game, viewport)
	self.camera1 = Camera()
	self.camera2 = Camera()

    def render(self):
        """Render the view to the given surface. This includes the game
           world, with transient objects such as players and flags
           """
        # draw left eye (blue-green)
	self.camera1 = self.camera
	self.camera1.azimuth -= 1
        glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_FALSE)
        self.camera1.load()
        self.scene.render()
	glClear(GL_DEPTH_BUFFER_BIT)
        # draw right eye (red)
	self.camera2 = self.camera
	self.camera2.azimuth += 1
        glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE)
        self.camera2.load()
        self.scene.render()
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE)

def attach(game, eventLoop):
    from BZFlag.UI.Viewport import OpenGLViewport
    viewport = OpenGLViewport(eventLoop, (800, 600))
    view = AnaglyphView(game, viewport)
    ThreeDController(view, viewport)

    # Add a Frame Hertz indicator
    Animated.FrequencyCounter(viewport.onFinishFrame,
                              lambda hz: "FHz: %.3f (target %.3f)" % (hz, viewport.targetFrameRate))

### The End ###

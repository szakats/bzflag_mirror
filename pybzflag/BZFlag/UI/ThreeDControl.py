""" BZFlag.UI.ThreeDControl

Controller classes for manipulating 3D views
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

import math
import pygame
from BZFlag import Animated
from BZFlag.UI import ThreeDView


class Viewing:
    """A controller for ThreeDView that provides rotation and panning through
       the level. This alone makes a good world viewer, and it is used as the
       foundation for a world editor.
       """
    def __init__(self, view, viewport):
        self.view = view
        self.viewport = viewport

        # Use Animated to smooth out our camera
        self.view.camera = ThreeDView.SmoothedCamera()

        self.mouseZoomScale = 1.08
        self.keyZoomScale = 1.6
        self.mouseRotateScale = 5.0
        view.camera.position = (0, 0, -20)

        def onMouseButtonDown(event):
            if event.button == 4:
                self.zoom(1/self.mouseZoomScale)
            if event.button == 5:
                self.zoom(self.mouseZoomScale)
        viewport.onMouseButtonDown.observe(onMouseButtonDown)

        def onKeyDown(event):
            if event.unicode == "f":
                self.toggleFullscreen()
            if event.unicode == "q":
                self.quit()
            if event.key == pygame.K_ESCAPE:
                self.quit()
            if event.unicode == "w":
                self.toggleWireframe()
            if event.unicode == "-":
                self.zoom(self.keyZoomScale)
            if event.unicode == "=":
                self.zoom(1/self.keyZoomScale)
        viewport.onKeyDown.observe(onKeyDown)

        def onMouseMotion(event):
            # Panning - shift-right mouse button
	    if pygame.key.get_mods() & pygame.KMOD_SHIFT:
	        if event.buttons[2]:
		    (x, y, z) = self.view.camera.position
                    radians = view.camera.azimuth * math.pi / 180
	 	    cos = math.cos(radians)
		    sin = math.sin(radians)
		    x +=  cos*event.rel[0] - sin*event.rel[1]
		    y += -sin*event.rel[0] - cos*event.rel[1]
		    self.view.camera.position = (x, y, z)
	            return

            # Elevation - ctrl-right mouse button
	    if pygame.key.get_mods() & pygame.KMOD_CTRL:
	        if event.buttons[2]:
		    (x, y, z) = self.view.camera.position
		    z -= event.rel[1]
		    if z > 0:
		         z = 0
		    if z < -1000:
		         z = -1000
		    self.view.camera.position = (x, y, z)
	            return

            # Rotation - right mouse button
            if event.buttons[2]:
                view.camera.azimuth += event.rel[0] / self.mouseRotateScale
                view.camera.elevation += event.rel[1] / self.mouseRotateScale
                if view.camera.elevation > 0:
                    view.camera.elevation = 0
                if view.camera.elevation < -90:
                    view.camera.elevation = -90
        viewport.onMouseMotion.observe(onMouseMotion)

    def toggleFullscreen(self):
        self.viewport.display.toggle_fullscreen()

    def toggleWireframe(self):
        self.viewport.wireframe = not self.viewport.wireframe

    def quit(self):
        self.viewport.eventLoop.stop()

    def zoom(self, scale):
        """Sets the camera zoom target to a multiple of its current
           value. The camera will smoothly track to this distance.
           """
        self.view.camera.distance *= scale
        if self.view.camera.distance < 0.1:
            self.view.camera.distance = 0.1
        if self.view.camera.distance > 1500:
            self.view.camera.distance = 1500


class Editing(Viewing):
    """Implement a superset of the Viewing controls, used for editing worlds"""
    def __init__(self, view, viewport):
        Viewing.__init__(self, view, viewport)

        def onMouseButtonDown(event):
            if event.button == 1:
                print 'picked: %r' % view.pick(event.pos)
        viewport.onMouseButtonDown.observe(onMouseButtonDown)

### The End ###

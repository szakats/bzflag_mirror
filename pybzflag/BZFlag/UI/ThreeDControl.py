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


class Viewing:
    """A controller for ThreeDView that provides rotation and panning through
       the level. This alone makes a good world viewer, and it is used as the
       foundation for a world editor.
       """
    def __init__(self, view, viewport):
        self.view = view
        self.viewport = viewport

        # Use Animated to smooth out our camera
        self.distance = Animated.Value(view.camera.distance,
                                       Animated.LogApproach(view.camera.distance, 4))
        self.focus = Animated.Vector(view.camera.position,
                                     Animated.LogApproach(view.camera.position, 10))
        self.azimuth = Animated.Value(view.camera.azimuth,
                                      Animated.LogApproach(view.camera.azimuth, 10))
        self.elevation = Animated.Value(view.camera.elevation,
                                        Animated.LogApproach(view.camera.elevation, 10))
        self.animated = [self.distance, self.focus, self.azimuth, self.elevation]

        self.mouseZoomScale = 1.08
        self.keyZoomScale = 1.6
        self.mouseRotateScale = 5.0
        view.camera.position = (0, 0, -20)

        # Update camera animation before each frame
        time = Animated.Timekeeper()
        def onSetupFrame():
            dt = time.step()
            for item in self.animated:
                item.integrate(dt)
            view.camera.distance = self.distance.value
            view.camera.position = self.focus.get()
        viewport.onSetupFrame.observe(onSetupFrame)

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
		    (x, y, z) = self.focus.f.target
                    radians = view.camera.azimuth * math.pi / 180
	 	    cos = math.cos(radians)
		    sin = math.sin(radians)
		    x +=  cos*event.rel[0] - sin*event.rel[1]
		    y += -sin*event.rel[0] - cos*event.rel[1]
		    self.focus.f.target = (x, y, z)
	            return

            # Elevation - ctrl-right mouse button
	    if pygame.key.get_mods() & pygame.KMOD_CTRL:
	        if event.buttons[2]:
		    (x, y, z) = self.focus.f.target
		    z -= event.rel[1]
		    if z > 0:
		         z = 0
		    if z < -1000:
		         z = -1000
		    self.focus.f.target = (x, y, z)
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
        self.distance.f.target *= scale
        if self.distance.f.target < 0.1:
            self.distance.f.target = 0.1
        if self.distance.f.target > 1500:
            self.distance.f.target = 1500

    def rotate(self, scale):
        self.rotation.f.target *= scale

### The End ###

""" BZFlag.UI.ThreeDView

A 3d scene renderer similar to BZFlag proper
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
from BZFlag import Event, Animated
from BZFlag.World import WorldObjects
from BZFlag.UI import Texture
from OpenGL.GL import *
from OpenGL.GLU import *
import sys


class Camera:
    """Abstraction for the OpenGL transforms used to set up the camera.
       For 1st person operation, use 'position', 'azimuth' and 'elevation',
       with 'distance' set to zero. For 3rd person operation, set 'distance'
       to a nonzero value.
       """
    def __init__(self):
        self.position  = (0, 0, 4.0)
        self.distance  = 900
        self.azimuth   = 180
        self.elevation = -75

    def load(self):
        """Set the current OpenGL matrix according to the camera's location and orientation"""
        glLoadIdentity()
        glTranslatef(0, 0, -self.distance)
        glRotatef(self.elevation, 1.0, 0.0, 0.0)
        glRotatef(self.azimuth, 0.0, 0.0, 1.0)
        glTranslatef(*self.position)


class Light:
    """Abstraction for an OpenGL light's position and intensity"""
    def __init__(self, lightnum):
        self.ambient  = (0, 0, 0, 1.0)
        self.diffuse  = (0, 0, 0, 1.0)
        self.position = (0, 0, 0, 1.0)
        self.lightnum = lightnum

    def set(self):
        """Set up and enable the light in the current OpenGL context"""
	glEnable(GL_LIGHTING)
        glLightfv(self.lightnum, GL_AMBIENT, self.ambient)
        glLightfv(self.lightnum, GL_DIFFUSE, self.diffuse)
        glLightfv(self.lightnum, GL_POSITION, self.position)
        glEnable(self.lightnum)


class Scene:
    """Set of all the objects this view sees, organized into rendering passes
       and sorted by texture. Multiple rendering passes are necessary to deal
       with blended objects, and texture sorting is an important OpenGL optimization.
       This class observes the current game state, making the necessary updates when
       the world is modified, players move, etc.
       """
    def __init__(self, game):
        self.game = game
        self.objects = {}
        self.passes = [{}, {}]

        def onLoadWorld():
            for block in self.game.world.blocks:
                if isinstance(block, WorldObjects.WorldObject):
                    self.objects[block] = block.getGLDrawables()
            self.rebuildTexmap()
        game.world.onLoad.observe(onLoadWorld)

    def rebuildTexmap(self):
        """Rebuilds the mapping from texture to object, used to sort objects
           by texture within each rendering pass.
           """
        for object, drawables in self.objects.items():
            for drawable in drawables:
                if drawable.blended:
                    if self.passes[1].has_key(drawable.texture):
                        self.passes[1][drawable.texture].append(drawable.draw)
                    else:
                        self.passes[1][drawable.texture] = [drawable.draw]
                else:
                    if self.passes[0].has_key(drawable.texture):
                        self.passes[0][drawable.texture].append(drawable.draw)
                    else:
                        self.passes[0][drawable.texture] = [drawable.draw]

    def render(self):
        """Render the scene to the current OpenGL context"""
        glDisable(GL_BLEND)
        for texture in self.passes[0].keys():
            glDisable(GL_TEXTURE_2D)
            if texture != None:
                glEnable(GL_TEXTURE_2D)
                texture.bind()
            for drawFunc in self.passes[0][texture]:
                drawFunc()
        glEnable(GL_BLEND)
        for texture in self.passes[1].keys():
            glDisable(GL_TEXTURE_2D)
            if texture != None:
                glEnable(GL_TEXTURE_2D)
                texture.bind()
            for drawFunc in self.passes[1][texture]:
                drawFunc()


class ThreeDView:
    """Shows a 3D view of the BZFlag game, renderable to an OpenGLViewport.
       This can be used for observer views, 1st person game views, and editing.
       """
    def __init__(self, game, viewport):
        self.game = game
        self.viewport = viewport
        self.camera = Camera()
        self.light0 = Light(GL_LIGHT0)
        self.light1 = Light(GL_LIGHT1)
        self.scene = Scene(game)

        # Initialize the opengl view
        self.light0.ambient  = (0.85, 0.85, 0.85, 1.0)
        self.light0.diffuse  = (0.85, 0.85, 0.85, 1.0)
        self.light0.position = (400, 400, 400, 1.0)
        self.light0.set()
        self.light1.ambient  = (0.85, 0.85, 0.85, 1.0)
        self.light1.diffuse  = (0.85, 0.85, 0.85, 1.0)
        self.light1.position = (0, 0, 400, 1.0)
        self.light1.set()

        glEnable(GL_DEPTH_TEST)
        glEnable(GL_NORMALIZE)
        glEnable(GL_CULL_FACE)
        glEnable(GL_COLOR_MATERIAL)
        glEnable(GL_LIGHTING)

        glClearColor(0.0, 0.0, 0.0, 0.0)
        glClearDepth(1.0)
        glDepthFunc(GL_LESS)
        glShadeModel(GL_SMOOTH)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

        viewport.setCaption("%s 3D View" % BZFlag.name)
        def onDrawFrame():
            game.update()
            self.render()
        viewport.onDrawFrame.observe(onDrawFrame)

    def render(self):
        """Render the view to the given surface. This includes the game
           world, with transient objects such as players and flags
           """
        self.camera.load()
        self.scene.render()


class ThreeDController:
    """A controller which attaches to the given view and viewport, providing camera control. """
    def __init__(self, view, viewport):
        self.view = view
        self.viewport = viewport
        self.distance = Animated.Value(view.camera.distance,
                                       Animated.LogApproach(view.camera.distance, 4))
        self.mouseZoomScale = 1.08
        self.keyZoomScale = 1.6
        self.mouseRotateScale = 5.0
        view.camera.position = (0, 0, -20)

	class modifiers:
	    pass
	modifiers.shift = False
	modifiers.alt = False
	modifiers.control = False

        time = Animated.Timekeeper()
        def onSetupFrame():
            dt = time.step()
            self.distance.integrate(dt)
            view.camera.distance = self.distance.value
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
            if event.unicode == "-":
                self.zoom(self.keyZoomScale)
            if event.unicode == "=":
                self.zoom(1/self.keyZoomScale)
        viewport.onKeyDown.observe(onKeyDown)

        def onMouseMotion(event):
	    if pygame.key.get_mods() & (pygame.KMOD_RSHIFT | pygame.KMOD_LSHIFT):
	        if event.buttons[1]:
		    (x, y, z) = view.camera.position
	 	    xscale = math.cos((view.camera.azimuth) * 3.1415926 / 180)
		    yscale = math.sin((view.camera.azimuth) * 3.1415926 / 180)
		    x += event.rel[0] * xscale - event.rel[1] * yscale
		    y += event.rel[0] * yscale - event.rel[1] * xscale
		    view.camera.position = (x, y, z)
	            return
	    if pygame.key.get_mods() & (pygame.KMOD_RALT | pygame.KMOD_LALT):
	        if event.buttons[1]:
		    (x, y, z) = view.camera.position
		    z -= event.rel[1]
		    if z > 0:
		         z = 0
		    if z < -1000:
		         z = -1000
		    view.camera.position = (x, y, z)
	            return
            if event.buttons[1]:
                view.camera.azimuth += event.rel[0] / self.mouseRotateScale
                view.camera.elevation += event.rel[1] / self.mouseRotateScale
                if view.camera.elevation > 0:
                    view.camera.elevation = 0
                if view.camera.elevation < -90:
                    view.camera.elevation = -90
        viewport.onMouseMotion.observe(onMouseMotion)

    def toggleFullscreen(self):
        self.viewport.display.toggle_fullscreen()

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

def attach(game, eventLoop):
    from BZFlag.UI.Viewport import OpenGLViewport
    viewport = OpenGLViewport(eventLoop, (800,600))
    view = ThreeDView(game, viewport)
    ThreeDController(view, viewport)

    # Add a Frame Hertz indicator
    Animated.FrequencyCounter(viewport.onFinishFrame,
                              lambda hz: "FHz: %.3f (target %.3f)" % (hz, viewport.targetFrameRate))

### The End ###

""" BZFlag.UI.ThreeDView

A 3d scene renderer similar to BZFlag proper
"""
#
# Python BZFlag Protocol Package
# Copyright (C) 2003 David Trowbridge <davidtrowbridge@users.sourceforge.net>
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
  def __init__(self):
    self.focus = (0, 0, 4.0)
    self.distance = 900
    self.rotation = 45
    self.elevation = -75
    self.isFocused = True

  def load(self):
    glLoadIdentity()
    glTranslatef(0, 0, -self.distance)
    glRotatef(self.elevation, 1.0, 0.0, 0.0)
    glRotatef(self.rotation, 0.0, 0.0, 1.0)
    if self.isFocused:
      glTranslatef(*self.focus)


class Light:
  def __init__(self, lightnum):
    self.ambient  = (0, 0, 0, 1.0)
    self.diffuse  = (0, 0, 0, 1.0)
    self.position = (0, 0, 0, 1.0)
    self.lightnum = lightnum

  def set(self):
    glLightfv(self.lightnum, GL_AMBIENT, self.ambient)
    glLightfv(self.lightnum, GL_DIFFUSE, self.diffuse)
    glLightfv(self.lightnum, GL_POSITION, self.position)
    glEnable(self.lightnum)


class Scene:
  def __init__(self, game):
    self.game = game
    game.world.onLoad.observe(self.onLoadWorld)
    self.objects = {}
    self.passes = [{}, {}]

  def onLoadWorld(self):
    for block in self.game.world.blocks:
      if isinstance(block, WorldObjects.WorldObject):
        self.objects[block] = block.getGLDrawables()
    self.rebuildTexmap()

  def rebuildTexmap(self):
    for object, drawables in self.objects.items():
      for drawable in drawables:
	if drawable.blended:
	  if self.passes[1].has_key(drawable.texture):
	    self.passes[1][drawable.texture].append(drawable)
	  else:
	    self.passes[1][drawable.texture] = [drawable]
	else:
	  if self.passes[0].has_key(drawable.texture):
	    self.passes[0][drawable.texture].append(drawable)
	  else:
	    self.passes[0][drawable.texture] = [drawable]

  def render(self):
    glDisable(GL_BLEND)
    for texture in self.passes[0].keys():
      glDisable(GL_TEXTURE_2D)
      if texture != None:
	glEnable(GL_TEXTURE_2D)
        texture.bind()
      for drawable in self.passes[0][texture]:
	drawable.draw()
    glEnable(GL_BLEND)
    for texture in self.passes[1].keys():
      glDisable(GL_TEXTURE_2D)
      if texture != None:
	glEnable(GL_TEXTURE_2D)
	texture.bind()
      for drawable in self.passes[1][texture]:
	drawable.draw()


class ThreeDView:
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

    glEnable(GL_LIGHTING)

    viewport.setCaption("%s 3D View" % BZFlag.name)
    def onDrawFrame():
      game.update()
      self.render()
    viewport.onDrawFrame.observe(onDrawFrame)

  def render(self):
    """Render the view to the given surface. This includes the game
       world, with transient objects such as players and flags"""
    self.camera.load()
    self.scene.render()


class ThreeDController:
  def __init__(self, view, viewport):
    time = Animated.Timekeeper()
    distance = Animated.Value(view.camera.distance,
                              Animated.LogApproach(view.camera.distance, 4))

    view.camera.focus = (0, 0, -90)

    def onSetupFrame():
      dt = time.step()
      distance.integrate(dt)
      view.camera.distance = distance.value
      view.camera.rotation += 6 * dt
    viewport.onSetupFrame.observe(onSetupFrame)

    def onMouseButtonDown(event):
      scale = 1.08
      if event.button == 4:
        distance.f.target /= scale
        if distance.f.target < 0.1:
          distance.f.target = 0.1
      if event.button == 5:
        distance.f.target *= scale
        if distance.f.target > 1500:
          distance.f.target = 1500
    viewport.onMouseButtonDown.observe(onMouseButtonDown)

    def onKeyDown(event):
      # f - toggle fullscreen
      if event.unicode == "f":
        viewport.display.toggle_fullscreen()
      # q - quit
      if event.unicode == "q":
        viewport.eventLoop.stop()
    viewport.onKeyDown.observe(onKeyDown)


def attach(game, eventLoop):
    from BZFlag.UI.Viewport import OpenGLViewport
    viewport = OpenGLViewport(eventLoop, (800,600))
    view = ThreeDView(game, viewport)
    ThreeDController(view, viewport)

    # Add a Frame Hertz indicator
    Animated.FrequencyCounter(viewport.onFinishFrame,
                              lambda hz: "FHz: %.3f (target %.3f)" % (hz, viewport.targetFrameRate))

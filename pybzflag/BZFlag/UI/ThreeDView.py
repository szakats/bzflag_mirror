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

import pygame, math
from pygame.locals import *
from BZFlag import Event
from BZFlag.World import Scene, WorldObjects
from BZFlag.UI import Texture
from OpenGL.GL import *
from OpenGL.GLU import *


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
      glTranslatef(self.focus[0], self.focus[1], self.focus[2])


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
    self.passes = {}

  def onLoadWorld(self):
    print "loading world..."
    for block in self.game.world.blocks:
      print 'block',id(block),'is a',block.__class__
      if isinstance(block, WorldObjects.WorldObject):
        self.objects[block] = block.getGLDrawables()
    self.rebuildTexmap()

  def rebuildTexmap(self):
    for object, drawables in self.objects.items():
      for drawable in drawables:
	if self.passes.has_key(drawable.texture):
	    self.passes[drawable.texture].append(drawable)
	else:
	    self.passes[drawable.texture] = [drawable]
	print 'drawable',drawable,'uses texture',drawable.texture

  def render(self):
    for texture in self.passes.keys():
      if texture != None:
        texture.bind()
      for drawable in self.passes[texture]:
	drawable.draw()


class ThreeDView:
  def __init__(self, game):
    self.game = game
    self.camera = Camera()
    self.light0 = Light(GL_LIGHT0)
    self.light1 = Light(GL_LIGHT1)
    self.scene = Scene(game)

  def configureOpenGL(self, size):
    glViewport(0, 0, size[0], size[1])
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45.0, size[0] / size[1], 3.0, 2500.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

  def initializeOpenGL(self, surface):
    """Initialize the opengl view"""
    self.size = surface.get_size()

    self.light0.ambient  = (0.85, 0.85, 0.85, 1.0)
    self.light0.diffuse  = (0.85, 0.85, 0.85, 1.0)
    self.light0.position = (400, 400, 400, 1.0)
    self.light0.set()
    self.light1.ambient  = (0.85, 0.85, 0.85, 1.0)
    self.light1.diffuse  = (0.85, 0.85, 0.85, 1.0)
    self.light1.position = (0, 0, 400, 1.0)
    self.light1.set()

    glEnable(GL_TEXTURE_2D)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_NORMALIZE)
    glEnable(GL_CULL_FACE)
    glEnable(GL_LIGHTING)

    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClearDepth(1.0)
    glDepthFunc(GL_LESS)
    glShadeModel(GL_SMOOTH)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

    glEnable(GL_LIGHTING)
    self.configureOpenGL(self.size)

  def render(self):
    """Render the view to the given surface. This includes the game
       world, with transient objects such as players and flags"""
    self.camera.load()
    self.scene.render()


def attach(game, eventLoop, size=(800,600), targetFrameRate=60):
  """Set up a window and opengl context on the given game and event loop"""

  def updateView():
    global view, screen
    if view:
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
      game.update()
      view.render()
      pygame.display.flip()
  eventLoop.add(Event.PeriodicTimer(1.0 / targetFrameRate, updateView))

  global view, screen
  pygame.init()
  screen = pygame.display.set_mode(size, pygame.OPENGL | pygame.DOUBLEBUF)
  pygame.display.set_caption("BZFlag 3D View")
  view = ThreeDView(game)
  view.initializeOpenGL(screen)
  updateView()

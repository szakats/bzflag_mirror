""" BZFlag.UI.RadarView

An overhead view implemented using OpenGL, optionally tracking
the position and orientation of one player.
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

import pygame, math
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
from BZFlag import Event


class RadarView:
    """An overhead view implemented using OpenGL, optionally tracking
       the position and orientation of one player.
       """
    def __init__(self, game):
        self.game = game
        
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
        glClearColor(0.0, 0.0, 0.0, 0.0)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        self.configureOpenGL(self.size)
        
    def render(self):
        glColor3f(1,1,1)
        glBegin(GL_POLYGON)
        glVertex3f(0,0,-20)
        glVertex3f(1,0,-20)
        glVertex3f(0,1,-20)
        glEnd()


def attach(game, eventLoop, size=(512,512), targetFrameRate=60):
    """Set up a window with only an overhead view, on the given game and event loop"""
 
    # Update the view regularly
    def updateView():
        global view, screen
        if view:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
            game.update()
            view.render()
            pygame.display.flip()
    eventLoop.add(Event.PeriodicTimer(1.0 / targetFrameRate, updateView))

    # Start up pygame and init the view
    global view, screen
    pygame.init()
    screen = pygame.display.set_mode(size, pygame.OPENGL | pygame.DOUBLEBUF)
    pygame.display.set_caption("BZFlag Radar View")
    view = RadarView(game)
    view.initializeOpenGL(screen)
    updateView()

### The End ###

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


# Color scheme for radar. Objects not found specifically default to None
colorScheme = {
    None:          (0.38, 0.62, 0.62),
    'Teleporter':  (1,    1,    0   ),
    }


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
        glEnable(GL_BLEND)
        glEnable(GL_LINE_SMOOTH)
        self.configureOpenGL(self.size)

    def colorScale(self, object, viewHeight):
        """This is the Enhanced Radar (tm) color scaling algorithm from bzflag proper"""
        z = object.center[2]
        h = object.size[2]
        if viewHeight >= z + h:
            alpha = 1.0 - (viewHeight - (z + h)) / 40.0
        elif viewHeight <= z:
            alpha = 1.0 - (z - viewHeight) / 40.0
        else:
            alpha = 1.0
        if alpha < 0.35:
            alpha = 0.35
        return alpha
        
    def render(self):
        glPushMatrix()
        glTranslatef(0,0,-1200)
        for object in self.game.world.blocks:
            try:
                try:
                    color = colorScheme[object.__class__.__name__]
                except KeyError:
                    color = colorScheme[None]
                glColor4f(color[0], color[1], color[2], self.colorScale(object, 0))
                
                # Render as a polygon then as an outline to get smoothed edges
                # without leaving a small crack between the triangles.
                poly = object.toPolygon()
                glBegin(GL_POLYGON)
                for vertex in poly:
                    glVertex2f(*vertex)
                glEnd()
                glBegin(GL_LINE_LOOP)
                for vertex in poly:
                    glVertex2f(*vertex)
                glEnd()
            except AttributeError:
                pass
        glPopMatrix()


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

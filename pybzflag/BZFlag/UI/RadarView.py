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
    None:           (0.38, 0.62, 0.62),
    'Teleporter':   (1,    1,    0   ),
    'player':       (1,    1,    1   ),
    'redPlayer':    (1,    0,    0   ),
    'greenPlayer':  (0,    1,    0   ),
    'bluePlayer':   (0,    0,    1   ),
    'purplePlayer': (0.5,  0,    1   ),
    }


class RadarView:
    """An overhead view implemented using OpenGL, optionally tracking
       the position and orientation of one player.
       """
    def __init__(self, game):
        self.game = game
        self.zoom = 1
        self.center = [0,0,0]
        self.angle = 0
        self.follow = None
        
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

    def colorScale(self, z, h, viewHeight):
        """This is the Enhanced Radar (tm) color scaling algorithm from bzflag proper"""
        if viewHeight >= z + h:
            alpha = 1.0 - (viewHeight - (z + h)) / 40.0
        elif viewHeight <= z:
            alpha = 1.0 - (z - viewHeight) / 40.0
        else:
            alpha = 1.0
        if alpha < 0.35:
            alpha = 0.35
        return alpha

    def smoothedPoly(self, poly):
        """Render a polygon with smoothed edges. This uses GL_POLYGON to render the
           interior without cracks between triangles, then GL_LINE_LOOP to render
           the edges smoothly.
           """
        glBegin(GL_POLYGON)
        for vertex in poly:
            glVertex2f(*vertex)
        glEnd()
        glBegin(GL_LINE_LOOP)
        for vertex in poly:
            glVertex2f(*vertex)
        glEnd()

    def polyDot(self, point, dotSize=5):
        """Convert a point to a polygon that draws a dot at that point.
           Currently this is always an axis-aligned square.
           """
        return ((point[0] - dotSize, point[1] - dotSize),
                (point[0] + dotSize, point[1] - dotSize),
                (point[0] + dotSize, point[1] + dotSize),
                (point[0] - dotSize, point[1] + dotSize))
      
    def renderWorld(self):
        for object in self.game.world.blocks:
            try:
                try:
                    color = colorScheme[object.__class__.__name__]
                except KeyError:
                    color = colorScheme[None]
                z = object.center[2]
                h = object.size[2]
                glColor4f(color[0], color[1], color[2], self.colorScale(z, h, 0))
                self.smoothedPoly(object.toPolygon())
            except AttributeError:
                pass

    def renderPlayers(self):
        for player in self.game.players.values():
            if 'alive' in player.status:
                self.follow = player
                
                # Pick player color based on team, alpha based on height
                try:
                    color = colorScheme[player.identity.team + "Player"]
                except KeyError:
                    color = colorScheme["player"]
                pos = player.motion.position
                glColor4f(color[0], color[1], color[2], self.colorScale(pos[2], 0, 0))
                self.smoothedPoly(self.polyDot(pos))

    def updateFollowing(self):
        if self.follow:
            self.center = self.follow.motion.position
            self.angle  = - self.follow.motion.azimuth * 180 / math.pi + 90
            self.zoom   = 2

    def render(self):
        glPushMatrix()
        glTranslatef(0,0,-10)
        self.updateFollowing()
        glScalef(0.01 * self.zoom, 0.01 * self.zoom, 1)
        glRotatef(self.angle, 0,0,1)
        glTranslatef(-self.center[0], -self.center[1], 0)
        self.renderWorld()
        self.renderPlayers()
        glPopMatrix()


def attach(game, eventLoop, size=(512,512), targetFrameRate=40):
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

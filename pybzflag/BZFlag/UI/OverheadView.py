""" BZFlag.UI.OverheadView

A 2D overhead view of the game, implemented using pygame.
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

class OverheadView:
    """Shows an overhead view of the BZFlag game, renderable to a pygame
       surface. Note that this observes the game state rather than the
       client, so it should be usable on either client or server side.
       """
    colorScheme = {
        'background': 'black',
        'Box':        '#408080',
        'Pyramid':    '#80FFFF',
        'Teleporter': '#FFFF80',
        'player':     'white',
      }
    
    def __init__(self, game):
        self.game = game
        self.cachedWorld = None

        # When the world is reloaded, invalidate our cached rendering of it
        def onLoadWorld():
            self.cachedWorld = None
        game.onLoadWorld.observe(onLoadWorld)

        # Replace the class colorScheme with an instance version that
        # translates everything into a proper pygame Color.
        convertedColors = {}
        for key in self.colorScheme:
            convertedColors[key] = Color(self.colorScheme[key])
        self.colorScheme = convertedColors
        
    def worldToView(self, point):
        """Convert world coordinates to view coordinates in pixels"""
        return ((point[0] / self.game.world.size[0] + 0.5) * self.size[0],
                (point[1] / self.game.world.size[1] + 0.5) * self.size[1])

    def objectToPoly(self, object):
        """Get a polygon representing the location of an object, given as a tuple of points.
           So far this will only return quadrilaterals.
           """
        # Create four vertices from the object's extents
        poly = ((object.center[0] - object.size[0]/2,
                 object.center[1] - object.size[1]/2),
                (object.center[0] + object.size[0]/2,
                 object.center[1] - object.size[1]/2),
                (object.center[0] + object.size[0]/2,
                 object.center[1] + object.size[1]/2),
                (object.center[0] - object.size[0]/2,
                 object.center[1] + object.size[1]/2))
        
        # Rotate the polygon by the object's angle
        cos = math.cos(object.angle)
        sin = math.sin(object.angle)
        def rotate(point):
            return ( cos*point[0] + sin*point[1],
                    -sin*point[0] + cos*point[1])
        return map(rotate, poly)

    def renderWorld(self, surface):
        """Render the game world to the given surface."""
        surface.fill(self.colorScheme['background'])
        for object in self.game.world.scene:
            color = self.colorScheme.get(object.__class__.__name__, None)
            if color:
                poly = map(self.worldToView, self.objectToPoly(object))
                pygame.draw.polygon(surface, color, poly)

    def render(self, surface):
        """Render the overhead view to the given surface. This includes the
           game world, with transient objects such as players and flags overlaid on top.
           """
        self.size = surface.get_size()

        # Since the game world doesn't usually change, we cache a rendered version of it.
        if (not self.cachedWorld) or self.cachedWorld.get_size() != surface.get_size():
            self.cachedWorld = pygame.Surface(surface.get_size()).convert()
            self.renderWorld(self.cachedWorld)
        surface.blit(self.cachedWorld, (0,0))

        # Now for the objects that change often
        self.renderFlags(surface)
        self.renderPlayers(surface)
        self.renderShots(surface)

    def renderPlayers(self, surface):
        color = self.colorScheme['player']
        for player in self.game.players.values():
            pos = self.worldToView(player.motion.position)
            pygame.draw.circle(surface, color, pos, 2)
        pass

    def renderFlags(self, surface):
        pass

    def renderShots(self, surface):
        pass


def simpleClient(client, size=(600,600), viewClass=OverheadView):
    """Set up the supplied client to display a window
       consisting only of an OverheadView.
       """
    import time
    
    # Update the view regularly
    global lastUpdate
    lastUpdate = None
    def updateView():
        global view, screen, lastUpdate
        if view:
            # FIXME: this isn't the right place to handle updating the game state!
            now = time.time()
            if lastUpdate:
                client.game.integrate(time.time() - lastUpdate)
            lastUpdate = now
            
            view.render(screen)
            pygame.display.flip()

    # FIXME: We need a real timer system
    client.eventLoop.pollTime = 0.01
    client.eventLoop.onPoll.observe(updateView)

    # Start up pygame when we first get world data
    global view, screen
    view = screen = None
    def initPygame():
        global view, screen
        if not view:
            pygame.init()
            screen = pygame.display.set_mode(size)
            pygame.display.set_caption("BZFlag Overhead View")
            view = OverheadView(client.game)
            updateView()
    client.onLoadWorld.observe(initPygame)
    
### The End ###
        
    

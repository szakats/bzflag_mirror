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
from BZFlag import Event


class OverheadView:
    """Shows an overhead view of the BZFlag game, renderable to a pygame
       surface. Note that this observes the game state rather than the
       client, so it should be usable on either client or server side.
       """
    colorScheme = {
        'background':    (88,  112, 88 ),
        'Box':           (188, 187, 167),
        'Pyramid':       (27,  141, 227),
        'Wall':          (255, 255, 0  ),
        'Teleporter':    (255, 255, 128),
        'playerOutline': (255, 255, 255),
        'flag':          (255, 255, 255),
        'roguePlayer':   (0,   0,   0  ),
        'redPlayer':     (128, 0,   0  ),
        'greenPlayer':   (0,   128, 0  ),
        'bluePlayer':    (0,   0,   128),
        'purplePlayer':  (96,  0,   144),
        'rabbitPlayer':  (128, 128, 128),
        'baseOutline':   (0  , 0  , 0  ),
        'redBase':       (128, 0,   0  ),
        'greenBase':     (0,   128, 0  ),
        'blueBase':      (0,   0,   128),
        'purpleBase':    (96,  0,   144),
      }

    def __init__(self, game):
        self.game = game
        self.cachedWorld = None

        # When the world is reloaded, invalidate our cached rendering of it
        def onLoadWorld():
            self.cachedWorld = None
        game.onLoadWorld.observe(onLoadWorld)

    def worldToView(self, point):
        """Convert world coordinates to view coordinates in pixels"""
        return (( point[0] * 0.98 / self.game.world.size[0] + 0.5) * self.size[0],
                (-point[1] * 0.98 / self.game.world.size[1] + 0.5) * self.size[1])

    def renderWorld(self, surface):
        """Render the game world to the given surface."""
        surface.fill(self.colorScheme['background'])
        for object in self.game.world.scene:
            objClassName = object.__class__.__name__
            if objClassName == 'TeamBase':
                # Color the bases by team
                color = self.colorScheme[object.team + 'Base']
                outline = self.colorScheme['baseOutline']
                poly = map(self.worldToView, object.toPolygon())
                pygame.draw.polygon(surface, color, poly)
                pygame.draw.lines(surface, outline, 1, poly)
            else:
                # Default representation
                color = self.colorScheme.get(objClassName, None)
                if color:
                    poly = map(self.worldToView, object.toPolygon())
                    if len(poly) > 2:
                        pygame.draw.polygon(surface, color, poly)
                    else:
                        pygame.draw.line(surface, color, poly[0], poly[1])

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
        color = self.colorScheme['playerOutline']
        for player in self.game.players.values():
            if 'alive' in player.status:
                # Pick player color based on team
                bg = self.colorScheme[player.identity.team + "Player"]

                # Draw a circle with black background and colored outline,
                # with a line indicating the tank's current heading.
                pos = self.worldToView(player.motion.position)
                height = player.motion.position[2]
                if height < 0:
                    height = 0
                size = height/4 + 6
                heading = (pos[0] + math.cos(-player.motion.azimuth)*size,
                           pos[1] + math.sin(-player.motion.azimuth)*size)
                pygame.draw.circle(surface, bg, pos, size-1)
                pygame.draw.circle(surface, color, pos, size, 1)
                pygame.draw.line(surface, color, pos, heading)

    def renderFlags(self, surface):
        color = self.colorScheme['flag']
        for flag in self.game.flags.values():
            if 'onGround' == flag.status:
                # Draw a little cross
                radius = 2
                pos = self.worldToView(flag.motion.position)
                pygame.draw.line(surface, color, (pos[0]-radius, pos[1]), (pos[0]+radius, pos[1]))
                pygame.draw.line(surface, color, (pos[0], pos[1]-radius), (pos[0], pos[1]+radius))

    def renderShots(self, surface):
        pass


def attach(game, eventLoop, size=(512,512), viewClass=OverheadView, targetFrameRate=30):
    """Set up a window with only an overhead view, on the given game and event loop"""

    # Update the view regularly
    def updateView():
        global view, screen
        if view:
            game.update()
            view.render(screen)
            pygame.display.flip()
    eventLoop.add(Event.PeriodicTimer(1.0 / targetFrameRate, updateView))

    # Start up pygame and init the view
    global view, screen
    pygame.init()
    screen = pygame.display.set_mode(size)
    pygame.display.set_caption("BZFlag Overhead View")
    view = OverheadView(game)
    updateView()

### The End ###

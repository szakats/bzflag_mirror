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

import pygame
from pygame.locals import *

class OverheadView(pygame.Surface):
    """A pygame surface that shows an overhead view of the BZFlag game.
       Note that this observes the game state rather than the client, so
       it should be usable on either client or server side.
       """
    def __init__(self, game, size):
        print game, size
        self.game = game
        pygame.Surface.__init__(self, size)


def simpleClient(client, size=(600,600), viewClass=OverheadView):
    """Set up the supplied client to display a window
       consisting only of an OverheadView.
       """

    # Start up pygame when we get world data
    screen = None
    view = None
    def initPygame():
        pygame.init()
        screen = pygame.display.set_mode(size)
        pygame.display.set_caption("BZFlag Overhead View")
        view = OverheadView(client.game, size)
    client.onLoadWorld.observe(initPygame)
    
    # Update the view regularly
    def updateView():
        if view:
            view.update()
            screen.blit(view, (0,0))
            pygame.display.flip()

### The End ###
        
    

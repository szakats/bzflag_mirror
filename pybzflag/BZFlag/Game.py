""" BZFlag.Game

Classes for holding the current game state. This includes the world,
all players, all flags. This is the hub around which the rest of this
package operates- clients and servers transmit and receive game state
over the network, frontends display and modify it.
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

from BZFlag import World, Util

class Game:
    """Container for all game state information.
       This object has events and functions for adding and removing
       objects from the game. Events for tracking modifications to an object
       are stored in that object's class.
       """
    def __init__(self):
        self.players = {}    # Indexed by player Id
        self.flags = {}      # Indexed by flag number (not Id)
        self.teams = {}      # Indexed by color, represented as a string
        self.world = World.World()
        Util.initEvents(self, 'onChangePlayerList', 'onAddPlayer', 'onRemovePlayer',
                        'onLoadWorld', 'onAddFlag')

    def integrate(self, dt):
        """Integrate velocity with respect to time everywhere it's needed"""
        for player in self.players.values():
            player.motion.integrate(dt)
        for flag in self.flags.values():
            flags.motion.integrate(dt)

    def addPlayer(self, player):
        self.players[player.identity.playerId] = player
        self.onAddPlayer(self, player)
        self.onChangePlayerList(self, self.players)

    def removePlayer(self, playerId):
        player = self.players[playerId]
        del self.players[playerId]
        self.onRemovePlayer(self, player)
        self.onChangePlayerList(self, self.players)

    def getFlag(self, number, cls=None):
        """Get a flag, creating it with the given class if necessary.
           There are no explicit messages for creating and destroying
           flags, they are implicitly created when the first flag update is sent.
           """
        flag = self.flags.get(number, None)
        if cls and not flag:
            flag = cls(number)
            self.flags[number] = flag
            self.onAddFlag(self, flag)
        return flag

### The End ###
        
    

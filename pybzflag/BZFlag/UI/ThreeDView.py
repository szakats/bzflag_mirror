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

import BZFlag
from BZFlag.UI import ThreeDRender, Drawable
from BZFlag.Protocol import WorldObjects
from OpenGL.GL import *


colorScheme = {
    'player':        (0,   0,   0  ),
    'redPlayer':     (1,   0,   0  ),
    'greenPlayer':   (0,   1,   0  ),
    'bluePlayer':    (0.2, 0.2, 1  ),
    'purplePlayer':  (1,   0,   1  ),
    'rabbitPlayer':  (1,   1,   1  ),
    }


class PlayerTransform(Drawable.Transform):
    """A drawable transform to set the tank's position, orientation, and color
       according to its attached player.
       """
    def __init__(self, player):
        self.player = player
        try:
            self.color = colorScheme[player.identity.team + "Player"]
        except KeyError:
            self.color = colorScheme['player']

    def apply(self):
        m = self.player.motion
        glPushMatrix()
        glTranslatef(*m.position)
        glRotatef(m.azimuth, 0,0,1)
        glColor3f(*self.color)

    def unapply(self):
        glColor3f(1,1,1)
        glPopMatrix()
    

class Tank:
    """A Scene object representing a player's tank"""
    def __init__(self, player): 
        # Load the tank from disk. The tank is divided into separate meshes
        # for the barrel, turret, body, and each tread.
        meshes = Drawable.VRML.load("tank.wrl")
        
        # Group all the tank's pieces together into one
        # display list, since we generally move them together.
        meshGroup = Drawable.Group(meshes.values())

        # Our final drawable is just a transformer wrapped around our mesh group
        self.player = player
        self.transforms = [PlayerTransform(player)]
        self.drawables = [Drawable.Transformer(meshGroup, self.transforms)]
        self.drawables[0].render.static = False

    def getDrawables(self):
        return self.drawables


class Scene(ThreeDRender.Scene):
    """A Scene class that builds on the functionality provided by ThreeDRender
       to provide a view of the current game world and other contents.
       This class observes the current game state, making the necessary updates when
       the world is modified, players move, etc.
       """
    def __init__(self, game):
        ThreeDRender.Scene.__init__(self)
        self.game = game
        self.playerTanks = {}

        game.world.onLoad.observe(self.reloadWorld)
        game.onAddPlayer.observe(self.addPlayer)
        game.onRemovePlayer.observe(self.removePlayer)
        game.onChangePlayerList.observe(self.changePlayerList)
        self.reloadWorld()

    def reloadWorld(self):
        """Rebuild the internal scene structures from the game's world"""
        self.erase()
        for block in self.game.world.blocks:
            if isinstance(block, WorldObjects.WorldObject):
                self.add(block)
        for player in self.game.players.itervalues():
            self.addPlayer(self.game, player)
        self.preprocess()

    def addPlayer(self, game, player):
        tank = Tank(player)
        self.playerTanks[player] = tank
        self.add(tank)

    def removePlayer(self, game, player):
        pass

    def changePlayerList(self, game, players):
        self.preprocess()


class ThreeDView(ThreeDRender.View):
    """Shows a 3D view of the BZFlag game, renderable to an OpenGLViewport.
       This can be used for observer views, 1st person game views, and editing.
       """
    def __init__(self, game, viewport):
        ThreeDRender.View.__init__(self, viewport, Scene(game))
        viewport.setCaption("%s 3D View" % BZFlag.name)

        def onDrawFrame():
            game.update()
        viewport.onDrawFrame.observe(onDrawFrame)


def attach(game, eventLoop):
    from BZFlag.UI import Viewport, ThreeDControl
    viewport = Viewport.OpenGLViewport(eventLoop, (800,600))
    view = ThreeDView(game, viewport)
    ThreeDControl.Viewing(view, viewport)
    return viewport

### The End ###

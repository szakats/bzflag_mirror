""" BZFlag.Server.Stateful

Provides the StatefulServer class, which extends BaseServer to
support updating a game state and transmitting changes.
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
from BZFlag import Event, Game, Flag
from StringIO import StringIO
from BZFlag.Server.Base import BaseServer


class StatefulServer(BaseServer):
    """On top of the basic networking functionality given by BaseServer,
       this implements a server that knows about a game state and can
       communicate this to clients.
       """
    def init(self):
        BaseServer.init(self)
        self.game = Game.Game()
        self.binaryWorld = None
        self.worldBlockSize = 1000

        self.options.update({
            'world': None,
            'welcomeMessage': BZFlag.serverWelcomeMessage,
            })

        Event.attach(self, 'onAttemptEnter', 'onEnter', 'onFinishEnter',
                     'onWelcome')

        def setOptions(**options):
            if 'world' in options.keys():
                f = open(options['world'])
                self.game.world.loadText(f)
                f.close()
        self.onSetOptions.observe(setOptions)

    def onMsgNegotiateFlags(self, msg):
        """Given a list of supported flags by the client and the
           flags in the game world, send back a list of flags the
           client is missing.
           """
        supportedFlags = Flag.splitAbbreviations(msg.data)
        missingFlags = []
        for flag in self.game.flags.values():
            if not flag.abbreviation in supportedFlags:
                missingFlags.append(flags)
        msg.socket.write(self.outgoing.MsgNegotiateFlags(
            data = Flag.joinAbbreviations(missingFlags)))

    def onMsgWantWHash(self, msg):
        """Send the client an MD5 hash of the current world"""
        msg.socket.write(self.outgoing.MsgWantWHash(
            lifetime = self.game.world.lifetime,
            hash = self.game.world.getHash()))

    def onMsgGetWorld(self, msg):
        """Send the client a section of our world"""
        # Convert the world to binary only on the first chunk we send
        if msg.offset == 0 or not self.binaryWorld:
            f = StringIO()
            self.game.world.saveBinary(f)
            self.binaryWorld = f.getvalue()

        block = self.binaryWorld[msg.offset:msg.offset + self.worldBlockSize]
        msg.socket.write(self.outgoing.MsgGetWorld(
            remaining = len(self.binaryWorld) - msg.offset - len(block),
            data = block))

    def onMsgEnter(self, msg):
        """Sent by the client to request entry into the game.
           the onEnter event provides a hook for denying the
           requests. Default is to allow.

           Events:

             onAttemptEnter(msg) - Sent when a client is trying to enter.
                                   Return a Common.RejectionCode string to
                                   veto the entry.

             onEnter(msg)        - Perform tasks necessary to enter the
                                   player into the game, such as sending
                                   initial score updates.

             onFinishEnter(msg)  - Send a MsgAddPlayer for the new player,
                                   ending the entering process.

             onWelcome(msg)      - Send a message to the player indicating
                                   that they have entered the game.
           """
        reason = self.onAttemptEnter(msg)
        if reason:
            # An observer vetoed the request, deny it
            # using the given reason
            msg.socket.write(self.outgoing.MsgReject(reason=reason))
        else:
            # Accept it. The player is now in the game.
            msg.socket.write(self.outgoing.MsgAccept())
            self.onEnter(msg)
            self.onFinishEnter(msg)
            self.onWelcome(msg)

    def onFinishEnter(self, msg):
        """Finish the process of entering a client into the game by
           sending it a MsgAddPlayer for itself.
           """
        msg.socket.write(self.outgoing.MsgAddPlayer(
            id           = msg.socket.id,
            type         = msg.playerType,
            team         = msg.team,
            wins         = 0,
            losses       = 0,
            teamKills    = 0,
            callSign     = msg.callSign,
            emailAddress = msg.emailAddress))

    def onWelcome(self, msg):
        """Event triggered after a player has completely entered the
           game, to welcome them in with a message.
           """
        msg.socket.write(self.outgoing.MsgMessage(
            fromId = 'server',
            toId = msg.socket.id,
            message = self.options['welcomeMessage']))

### The End ###

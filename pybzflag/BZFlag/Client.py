""" BZFlag.Client

Provides the BaseClient class, which implements basic communication
with the server and provides hooks for adding more functionality
in subclasses.

Provides subclasses providing logic to update the game state,
and providing player functionality.
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
from BZFlag import Network, Protocol, Errors, Player, Game, World, Event, Flag
from StringIO import StringIO


class BaseClient(Network.Endpoint):
    """A basic client class that doesn't process any messages."""

    outgoing = Protocol.ToServer
    incoming = Protocol.FromServer
    
    def init(self):
        Network.Endpoint.init(self)
        self.connected = 0
        self.options.update({
            'server': None,
            })
        Event.attach(self, 'onConnect', 'onDisconnect')

        def setOptions(**options):
            if 'server' in options.keys():
                self.connect(options['server'])
        self.onSetOptions.observe(setOptions)

    def connect(self, server):
        """This does the bare minimum necessary to connect to the
           BZFlag server. It does not negotiate flags, obtain the
           world database, or join the game. After this function
           returns, the client is connected to the server and can
           receive and transmit messages.
           """
        if not server:
            raise Errors.NetworkException("A server must be specified")

        # Establish the TCP socket
        if self.tcp:
            self.disconnect()
        self.tcp = Network.Socket()
        self.tcp.connect(server, Protocol.Common.defaultPort)
        self.tcp.setBlocking(0)
        self.eventLoop.add(self.tcp)

        # Until we establish a UDP connection, we'll need to send
        # normally-multicasted messages over TCP
        self.multicast = self.tcp

        # Now we have to wait for the server's Hello packet,
        # with the server version and client ID.
        self.tcp.handler = self.handleHelloPacket

    def disconnect(self):
        if self.tcp:
            self.tcp.close()
            self.eventLoop.remove(self.tcp)
            self.tcp = None
        if self.udp:
            self.udp.close()
            self.eventLoop.remove(self.udp)
            self.udp = None
        self.multicast = None
        self.connected = 0
        self.onDisconnect()

    def onDisconnect(self):
        self.eventLoop.stop()

    def handleHelloPacket(self, socket, eventLoop):
        """This is a callback used to handle incoming socket
           data when we're expecting a hello packet.
           """
        # We should have just received a Hello packet with
        # the server version and our client ID.
        hello = socket.readStruct(self.incoming.HelloPacket)
        if hello.version != BZFlag.protocolVersion:
            raise Errors.ProtocolError(
                "Protocol version mismatch: The server is version " +
                "'%s', this client is version '%s'." % (
                hello.version, BZFlag.protocolVersion))
        self.id = hello.clientId

        # Now we're connected
        self.connected = 1
        socket.handler = self.handleMessage
        self.onConnect()


class StatefulClient(BaseClient):
    """Extends the BaseClient to keep track of the state of the game
       world, as reported by the server and the other clients.
       """
    def init(self):
        BaseClient.init(self)
        self.game = Game.Game()
        self.worldCache = None
        Event.attach(self, 'onLoadWorld', 'onStartWorldDownload',
                     'onNegotiateFlags')

    def onMsgSuperKill(self, msg):
        """The server wants us to die immediately"""
        self.disconnect()

    def onMsgLagPing(self, msg):
        """The server is measuring our lag, reply with the same message."""
        msg.socket.write(msg)

    def onMsgNetworkRelay(self, msg):
        """The server needs us to use TCP instead of UDP for messages
           that we'd normally multicast.
           """
        self.multicast = self.tcp

    def onConnect(self):
        """Immediately after connecting, ask for a world hash so
           we can check our cache for a copy of that world
           """
        self.negotiateFlags()

    def negotiateFlags(self):
        """Send a MsgNegotiateFlags to the server to indicate which
           flags we know about. Either this will cause a MsgNull to
           come back indicating that our flag dataset is incomplete,
           or we will receive a MsgNegotiateFlags back with flag IDs
           to use for this connection.
           """
        # One character abbreviations must be null-padded to two characters
        def padAbbreviation(str):
            if len(str) < 2:
                return str + chr(0)
            return str
        flagAbbreviations = map(padAbbreviation, Flag.getDict().keys())
        self.tcp.write(self.outgoing.MsgNegotiateFlags(
            numFlags = len(flagAbbreviations),
            data = "".join(flagAbbreviations),
            ))

    def onMsgNull(self, msg):
        """This should only be sent when flag negotiation fails.
           In fact, this shouldn't be used at all according to the protocol spec.
           The fact that this message is used for flag negotiaions should be
           considered a bug in the protocol.
           """
        raise Errors.ProtocolError("Flag negotiation failed")

    def onMsgNegotiateFlags(self, msg):
        """Flag negotiation succeeded. This message tells us what IDs we should
           use to represent each flag over the wire.

           The message's data is a variable length list of FlagNegotiationID
           structures that map an ID and abbreviation. This looks up those
           abbreviations in the Flag module and creates dictionaries mapping
           those classes to IDs and vice versa.
           """
        data = StringIO(msg.data)
        self.flagIdToClass = {}
        self.flagClassToId = {}
        flagDict = Flag.getDict()
        unknownFlags = []
        for i in xrange(msg.numFlags):
            flag = Protocol.Common.FlagNegotiationID()
            flag.read(data)
            try:
                cls = flagDict[flag.abbreviation]
            except KeyError:
                unknownFlags.append(flag.abbreviation)
            self.flagIdToClass[flag.id] = cls
            self.flagClassToId[cls] = flag.id
        self.onNegotiateFlags()
        if unknownFlags:
            raise Errors.ProtocolWarning(
                "Server knows about the following flags that we don't: %s" %
                " ".join(unknownFlags))

    def onNegotiateFlags(self):
        """Immediately after flag negotiation we usually want to start downloading
           the world. The first step is to get the world hash, so we can see if we
           have a cached copy of it.
           """
        self.getWorldHash()

    def getWorldHash(self):
        """Ask for a hash of the binary world data, so we can
           check our cache for it. This will trigger onMsgWantHash()
           """
        self.tcp.write(self.outgoing.MsgWantWHash())

    def onMsgWantWHash(self, msg):
        """Receive the world hash. If this is a permanent
           (not automatically generated by bzfs) map, try
           to cache it.
           """
        if msg.lifetime == 'permanent':
            self.worldCache = World.Cache()
        else:
            self.worldCache = None
        self.worldHash = msg.hash

        if self.worldCache and self.worldCache.hasWorld(self.worldHash):
            # Yay, the world is in our cache
            f = self.worldCache.openWorld(self.worldHash)
            self.game.world.loadBinary(f)
            f.close()
            self.onLoadWorld()
        else:
            # We're not using the cache or it didn't have our world.
            # Start a download.
            self.downloadWorld()

    def downloadWorld(self):
        """Start downloading the game world from the server.
           This will trigger onMsgGetWorld(), which will send
           more MsgGetWorlds until the entire world has been downloaded.
           """
        self.onStartWorldDownload()
        self.binaryWorld = ''
        self.tcp.write(self.outgoing.MsgGetWorld(offset=0))

    def onMsgGetWorld(self, msg):
        """We've received one chunk of the binary world. If there's more,
           request it, otherwise load the world and move on.
           """
        self.binaryWorld += msg.data
        if msg.remaining:
            # We need more data!
            self.tcp.write(self.outgoing.MsgGetWorld(offset=len(self.binaryWorld)))
        else:
            # Download is complete. Convert the binary world
            # into a World object and discard the binary world.
            # If we're using a cache, save a copy of the map.
            if self.worldCache:
                self.worldCache.storeWorld(self.worldHash, self.binaryWorld)
            import StringIO
            self.game.world.loadBinary(StringIO.StringIO(self.binaryWorld))
            del self.binaryWorld
            self.worldDownloaded = 1
            self.onLoadWorld()

    def onMsgAddPlayer(self, msg):
        self.game.addPlayer(Player.fromMessage(msg))

    def onMsgRemovePlayer(self, msg):
        self.game.removePlayer(msg.id)

    def onMsgPlayerUpdate(self, msg):
        self.game.players[msg.id].updateFromMessage(msg)

    def updateFlag(self, msg):
        """Generic handler for all messages that update a flag"""
        try:
            flag = self.game.getFlag(msg.flagNum, self.flagIdToClass[msg.update.id])
            flag.updateFromMessage(msg)
        except KeyError:
            raise Errors.ProtocolWarning("Can't update flag number %d with unknown ID %d" %
                                         (msg.flagNum, msg.update.id))

    def onMsgFlagUpdate(self, msg):
        self.updateFlag(msg)

    def onMsgGrabFlag(self, msg):
        self.updateFlag(msg)

    def onMsgDropFlag(self, msg):
        self.updateFlag(msg)

    def onMsgTeamUpdate(self, msg):
        # FIXME
        pass

    def onMsgNewRabbit(self, msg):
        # FIXME
        pass

    def onMsgShotBegin(self, msg):
        # FIXME
        pass

    def onMsgShotEnd(self, msg):
        # FIXME
        pass

    def onMsgAlive(self, msg):
        # FIXME
        pass

    def onMsgTeleport(self, msg):
        # FIXME
        pass


class PlayerClient(StatefulClient):
    """Extends the StatefulClient with functionality for implementing
       a player. This includes methods for entering and leaving the
       game, and a method of glueing the client with a frontend that
       provides actual player interaction, or a bot AI.
       """
    def init(self):
        StatefulClient.init(self)
        self.options.update({
            'identity': None,    # A Player.Identity instance
            })

        # We won't have a player instance until we get a MsgAddPlayer
        # back for ourselves and StatefulClient adds it to the Game.
        # We do need to store the identity used for joining the game
        # of course.
        self.inGame = 0
        self.player = None
        Event.attach(self, 'onEnterGame', 'onInitPlayer')

        # Set up an observer for the game's onAddPlayer that will set
        # up our player member whenever that MsgAddPlayer is received.
        def onAddPlayer(game, player):
            if player.identity.playerId == self.id:
                self.player = player
                self.onInitPlayer()
        self.game.onAddPlayer.observe(onAddPlayer)

    def onLoadWorld(self):
        self.game.onLoadWorld()
        self.enterGame()

    def enterGame(self):
        msg = self.outgoing.MsgEnter()
        identity = self.options['identity']
        msg.playerType = identity.type
        msg.team = identity.team
        msg.callSign = identity.callSign
        msg.emailAddress = identity.emailAddress
        self.tcp.write(msg)

    def disconnect(self):
        self.exitGame()
        StatefulClient.disconnect(self)

    def exitGame(self):
        self.inGame = 0
        self.tcp.write(self.outgoing.MsgExit())

    def onMsgAccept(self, msg):
        """This is called after we try to enterGame, if it's successful."""
        self.inGame = 1
        self.onEnterGame()

    def onMsgReject(self, msg):
        """This is called after we try to enterGame, if we failed."""
        raise Errors.GameException("Unable to enter the game: %s" % msg.reason)

    def sendMessage(self, message, destination='all'):
        """Send a message to other players"""
        self.tcp.write(self.outgoing.MsgMessage(destination = destination,
                                                message = message))

### The End ###

""" BZFlag.Client

Provides the BaseClient class, which implements basic communication
with the server and provides hooks for adding more functionality
in subclasses.
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
from BZFlag import Network, Protocol, Errors, Player, Game, World, Util, Flag
from BZFlag.Protocol import FromServer, ToServer, Common
from StringIO import StringIO


class BaseClient:
    """Implements a very simple but extensible BZFlag client.
       This client can connect and disconnect, and it has a system
       for asynchronously processing messages. This class only processes
       messages related to upkeep on the server-client link, such as
       lag ping, disconnection, and UDP-related messages.

       The methods of this class and its subclasses use the following
       naming conventions:

         - Low-level socket handlers should be of the form handleFoo()
         - Event handlers for messages should be of the form onMsgFoo()
         - Event handlers for other events should be of the form onFoo()

       All onFoo() and onMsgFoo() events are observable and traceable,
       See the Util.Event class for more information about this feature.
       """

    def __init__(self, **options):
        """Any options passed to the constructor will be sent to setOptions"""
        self.tcp = None
        self.udp = None
        self.connected = 0
        self.options = {
            'server': None,
            }

        Util.initEvents(self, 'onConnect', 'onAnyMessage', 'onUnhandledMessage',
                        'onDisconnect')

        # Add events for all messages, with onUnhandledMessage as an
        # unhandled event handler.
        for message in Common.getMessageDict(FromServer).values():
            eventName = self.getMsgHandlerName(message)
            if hasattr(self, eventName):
                event = Util.Event(getattr(self, eventName))
            else:
                event = Util.Event()
            event.unhandledCallback = self.onUnhandledMessage
            setattr(self, eventName, event)
            
        self.init()
        self.setOptions(**options)

    def getMsgHandlerName(self, messageClass):
        return "on%s" % messageClass.__name__

    def init(self):
        """A hook for subclasses to add initialization in the proper sequence"""
        pass

    def setOptions(self, **options):
        self.options.update(options)

        if 'eventLoop' in options.keys():
            self.eventLoop = options['eventLoop']
        else:
            self.eventLoop = Network.EventLoop()

        if 'server' in options.keys():
            self.connect(options['server'])
            
    def getSupportedOptions(self):
        return self.options.keys()

    def connect(self, server):
        """This does the bare minimum necessary to connect to the
           BZFlag server. It does not negotiate flags, obtain the
           world database, or join the game. After this function
           returns, the client is connected to the server and can
           receive and transmit messages.
           """
        # Establish the TCP socket
        if self.tcp:
            self.disconnect()
        self.tcp = Network.Socket()
        self.tcp.connect(server, Common.defaultPort)
        self.tcp.setBlocking(0)
        self.eventLoop.registerSocket(self.tcp)

        # Until we establish a UDP connection, we'll need to send
        # normally-multicasted messages over TCP
        self.multicast = self.tcp

        # Now we have to wait for the server's Hello packet,
        # with the server version and client ID.
        self.tcp.handler = self.handleHelloPacket

    def disconnect(self):
        if self.tcp:
            self.tcp.close()
            self.eventLoop.unregisterSocket(self.tcp)
            self.tcp = None
        if self.udp:
            self.udp.close()
            self.udp = None
        self.multicast = None
        self.connected = 0
        self.onDisconnect()

    def run(self):
        self.eventLoop.run()

    def handleHelloPacket(self, socket, eventLoop):
        """This is a callback used to handle incoming socket
           data when we're expecting a hello packet.
           """
        # We should have just received a Hello packet with
        # the server version and our client ID.
        hello = socket.readStruct(FromServer.HelloPacket)
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

    def handleMessage(self, socket, eventLoop):
        """This is a callback used to handle incoming socket
           data when we're expecting a message.
           """
        # This can return None if part of the mesasge but not the whole
        # thing is available. The rest of the message will be rebuffered,
        # so we'll read the whole thing next time this is called.
        msg = socket.readMessage(FromServer)
        if msg:
            msg.socket = socket
            msg.eventLoop = eventLoop
            handler = getattr(self, self.getMsgHandlerName(msg.__class__))
            if self.onAnyMessage(msg):
                return
            handler(msg)

    def onUnhandledMessage(self, msg):
        raise Errors.ProtocolWarning("Unhandled message %s" % msg.__class__.__name__)
    
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

        
class StatefulClient(BaseClient):
    """Extends the BaseClient to keep track of the state of the game
       world, as reported by the server and the other clients.
       """
    def init(self):
        BaseClient.init(self)
        self.game = Game.Game()
        self.worldCache = None
        Util.initEvents(self, 'onLoadWorld', 'onStartWorldDownload',
                        'onNegotiateFlags')

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
        self.tcp.write(ToServer.MsgNegotiateFlags(
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
        for i in xrange(msg.numFlags):
            flag = Common.FlagNegotiationID()
            flag.read(data)
            try:
                cls = flagDict[flag.abbreviation]
            except KeyError:
                raise Errors.ProtocolError("Server returned an unknown flag after negotiation")
            self.flagIdToClass[flag.id] = cls
            self.flagClassToId[cls] = flag.id
        self.onNegotiateFlags()

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
        self.tcp.write(ToServer.MsgWantWHash())
        
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
        self.tcp.write(ToServer.MsgGetWorld(offset=0))

    def onMsgGetWorld(self, msg):
        """We've received one chunk of the binary world. If there's more,
           request it, otherwise load the world and move on.
           """
        self.binaryWorld += msg.data
        if msg.remaining:
            # We need more data!
            self.tcp.write(ToServer.MsgGetWorld(offset=len(self.binaryWorld)))
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

    def onMsgFlagUpdate(self, msg):
        pass

    def onMsgTeamUpdate(self, msg):
        pass

    def onMsgNewRabbit(self, msg):
        pass

    def onMsgAlive(self, msg):
        pass

    def onMsgShotBegin(self, msg):
        pass

    def onMsgGrabFlag(self, msg):
        pass

    def onMsgDropFlag(self, msg):
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
        Util.initEvents(self, 'onEnterGame', 'onInitPlayer')

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
        msg = ToServer.MsgEnter()
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
        self.tcp.write(ToServer.MsgExit())

    def onMsgAccept(self, msg):
        """This is called after we try to enterGame, if it's successful."""
        self.inGame = 1
        self.onEnterGame()

    def onMsgReject(self, msg):
        """This is called after we try to enterGame, if we failed."""
        raise Errors.GameException("Unable to enter the game: %s" % msg.reason)


### The End ###
        
    

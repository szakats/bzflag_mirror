""" BZFlag.Server

Provides the BaseServer class, which implements basic communication
with clients and provides hooks for adding more functionality
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
from BZFlag import Network, Protocol, Event, Errors, Game, Flag
from StringIO import StringIO


class BaseServer(Network.Endpoint):
    """On top of the basic message processing provided by Network.Endpoint,
       this class manages connections from clients.
       """

    outgoing = Protocol.FromServer
    incoming = Protocol.ToServer
    
    def init(self):
        Network.Endpoint.init(self)
        self.clients = {}
        self.nextClientID = 0
        self.clientIDIncrement = 1
        self.options.update({
            'interface': None,
            })
        Event.attach(self, 'onConnect', 'onDisconnect')

        def setOptions(**options):
            if 'interface' in options.keys():
                self.listen(options['interface'])
        self.onSetOptions.observe(setOptions)

    def listen(self, interface):
        self.tcp = Network.Socket()
        self.tcp.listen(interface, Protocol.Common.defaultPort)
        self.tcp.setBlocking(0)
        self.tcp.handler = self.handleConnection
        self.eventLoop.add(self.tcp)

    def getNewClientID(self):
        id = self.nextClientID
        self.nextClientID += self.clientIDIncrement
        return id

    def handleConnection(self, socket, eventLoop):
        """This is called when a new connection to the server is made.
           It sends the hello packet, and sets up a new client socket.
           """
        # On a new client connection, send them the
        # hello packet and add to our dict of clients.
        clientSocket = socket.accept()
        clientId = self.getNewClientID()
        clientSocket.id = clientId
        self.clients[clientId] = clientSocket
        hello = self.outgoing.HelloPacket(version = BZFlag.protocolVersion,
                                          clientId = clientId)
        self.eventLoop.add(clientSocket)
        clientSocket.setBlocking(0)
        clientSocket.handler = self.handleMessage
        clientSocket.write(hello)
        self.onConnect(clientSocket)

    def handleMessage(self, socket, eventLoop):
        """This is called when new data is received from the client.
           It is a wrapper around Network.Endpoint.handleMessage which
           catches ConnectionLost exceptions and disconnects the client
           safely.
           """
        try:
            Network.Endpoint.handleMessage(self, socket, eventLoop)
        except Errors.ConnectionLost:
            try:
                del self.clients[socket.id]
                self.eventLoop.remove(socket)
                self.onDisconnect(socket)
            except KeyError:
                # Already disconnected
                pass


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
            })

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


class StandardServer(StatefulServer):
    """A server that implements standard game rules in the world supplied
       by a StatefulServer
       """
    pass

### The End ###

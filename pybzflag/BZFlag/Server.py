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
from BZFlag import Network, Protocol, Event, Errors


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
        self.nextClientID += 1
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
                                          clientId = self.getNewClientID())
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

### The End ###

""" BZFlag.Client

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
from BZFlag import Network, Protocol, Errors, Player, Game, World, Event, Flag
from BZFlag.Protocol import FromServer, ToServer, Common
from StringIO import StringIO


class BaseServer:
    """Implements a very simple but extensible BZFlag server.
       This client can accept connections from clients, and it has a system
       for asynchronously processing messages. This class does not provide
       any message handler implementations.

       The methods of this class and its subclasses use the following
       naming conventions:

         - Low-level socket handlers should be of the form handleFoo()
         - Event handlers for messages should be of the form onMsgFoo()
         - Event handlers for other events should be of the form onFoo()

       All onFoo() and onMsgFoo() events are observable and traceable,
       See the Event class for more information about this feature.
       """
    
    def __init__(self, **options):
        """Any options passed to the constructor will be sent to setOptions"""
        self.tcp = None
        self.udp = None
        self.clients = []
        self.options = {
            'interface': None,
            }

        Event.attach(self, 'onConnect', 'onAnyMessage', 'onUnhandledMessage',
                     'onDisconnect')

        # Add events for all messages, with onUnhandledMessage as an
        # unhandled event handler.
        for message in Common.getMessageDict(ToServer).values():
            eventName = self.getMsgHandlerName(message)
            if hasattr(self, eventName):
                event = Event.Event(getattr(self, eventName))
            else:
                event = Event.Event()
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
            self.eventLoop = Event.EventLoop()

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
        self.eventLoop.add(self.tcp)

        # Until we establish a UDP connection, we'll need to send
        # normally-multicasted messages over TCP
        self.multicast = self.tcp

        # Now we have to wait for the server's Hello packet,
        # with the server version and client ID.
        self.tcp.handler = self.handleHelloPacket


### The End ###

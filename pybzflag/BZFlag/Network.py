""" BZFlag.Network

Network transport layer for BZFlag clients and servers. This
provides methods for transporting messges across TCP and UDP
links.
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

from BZFlag.Protocol import Common
from BZFlag import Protocol, Errors
import socket, sys


class Socket:
    """This is a socket wrapper that can be used for normal socket
       operations, but also supports sending BZFlag messages.
       """
    def __init__(self, protocol='TCP'):
        self.readBuffer = ''
        if protocol:
            self.socket = getattr(self, "new%sSocket" % protocol)()

    def newTCPSocket(self):
        tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.setNodelay(tcp)
        return tcp
        
    def newUDPSocket(self):
        return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def setNodelay(self, s):
        # Disable the Nagle algorithm. This is necessary to get
        # anything near reasonable latency when sending small packets.
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)

    def close(self):
        self.socket.close()
        self.socket = None

    def setBlocking(self, flag):
        self.socket.setblocking(flag)

    def write(self, data):
        try:
            self.socket.send(str(data))
        except socket.error:
            raise Errors.ConnectionLost()

    def recv(self, size):
        """Low-level recv() wrapper that just provides a little error handling."""
        try:
            data = self.socket.recv(size)
        except socket.error:
            # Assume this is "resource temporarily unavailable"
            return None
        if not data:
            raise Errors.ConnectionLost()
        return data

    def read(self, size=None, bufferSize=64*1024):
        """High level interface for reading from the socket,
           includes a buffering scheme that works well for
           receiving fixed size messages.
           """
        if size is None:
            # Keep reading until there's no more to read
            received = self.readBuffer
            self.readBuffer = ''
            try:
                while 1:
                    received += self.recv(bufferSize)
            except Errors.ConnectionLost:
                pass
            return received
        elif size != 0:
            # Read the amount specified, first from our
            # local read buffer then from the socket itself.
            # If we can't read the complete packet, return
            # None and keep the partial packet buffered.
            while len(self.readBuffer) < size:
                chunk = self.recv(size - len(self.readBuffer))
                if not chunk:
                    return None
                self.readBuffer += chunk
            received = self.readBuffer[:size]
            self.readBuffer = self.readBuffer[size:]
            return received
        else:
            # Zero size
            return ''

    def unread(self, data):
        """Push the supplied data back into the read buffer,
           so that it will be the next thing read.
           """
        self.readBuffer = str(data) + self.readBuffer

    def connect(self, host, port=None):
        """The port can be specified either as part of the host using
           a hostname:port format, or as a separate argument. A port
           included in the host overrides the port in the arguments
           if present, so you can treat the port argument as a default.
           """
        if host.find(":") >= 0:
            (host, port) = host.split(":")
            port = int(port)
        self.socket.connect((host, port))

    def listen(self, host, port=None):
        """The arguments for this are processed just like for connect(),
           plus a blank hostname is interpreted as all interfaces.
           To listen on a different port on all interfaces for example,
           you can use a hostname like ':1234'
           """
        if host.find(":") >= 0:
            (host, port) = host.split(":")
            port = int(port)
        if not host:
            host = socket.gethostname()

        # Avoid having this port clogged up for a while after the server dies
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        
        self.socket.bind((host, port))
        self.interface = host
        self.port = port
        self.socket.listen(5)

    def accept(self):
        """Accept an incoming connection on a socket that has been bind()'ed,
           returning a Socket class for the new connection.
           """
        s = Socket(None)
        (s.socket, s.address) = self.socket.accept()
        self.setNodelay(s.socket)
        return s

    def poll(self, eventLoop):
        """This is designed for event driven programming with
           one or more sockets. The event loop should call this
           method when it detects activity on the socket, then
           this method will call the current socket handler.
           If there really wasn't any data available (assuming
           this is a nonblocking socket) it will return
           uneventfully.
           """
        self.handler(self, eventLoop)

    def getSelectable(self):
        """Return an object for this socket that can be passed
           to select()
           """
        return self.socket.fileno()

    def readStruct(self, structClass):
        struct = structClass()
        packed = self.read(struct.getSize())
        if not packed:
            return None
        struct.unmarshall(packed)
        return struct

    def readMessage(self, msgModule):
        """Read a message, using the supplied module full of Message subclasses."""
        header = self.readStruct(Common.MessageHeader)
        if not header:
            return None
        if header.length > 0:
            body = self.read(header.length)
            if not body:
                # Save the header for later if we can't read the body yet
                self.unread(header)
                return None
        else:
            body = ''
        try:
            msgClass = Common.getMessageDict(msgModule)[header.id]
        except KeyError:
            raise Errors.ProtocolWarning("Received %s byte message with unknown type 0x%04X in %s" %
                                         (header.length, header.id, msgModule.__name__))
        return msgClass(str(header) + body)


class Endpoint:
    """Abstract base class for an endpoint in the BZFlag client-server connection.
       This is a base class for both BaseClient and BaseServer.

       This class provides a system for asynchronously processing messages
       and dispatching them to event handler functions.

       The methods of this class and its subclasses use the following
       naming conventions:

         - Low-level socket handlers should be of the form handleFoo()
         - Event handlers for messages should be of the form onMsgFoo()
         - Event handlers for other events should be of the form onFoo()

       All onFoo() and onMsgFoo() events are observable and traceable,
       See the Event class for more information about this feature.      
       """
    
    # Protocol modules for the messages sent in and out of this endpoint
    outgoing = None
    incoming = None
    
    def __init__(self, **options):
        """Any options passed to the constructor will be sent to setOptions"""
        self.tcp = None
        self.udp = None
        self.options = {}
        
        from BZFlag import Event
        Event.attach(self, 'onAnyMessage', 'onUnhandledMessage', 'onSetOptions')

        # Add events for all messages, with onUnhandledMessage as an
        # unhandled event handler.
        for message in Common.getMessageDict(self.incoming).values():
            eventName = self.getMsgHandlerName(message)
            if hasattr(self, eventName):
                event = Event.Event(getattr(self, eventName))
            else:
                event = Event.Event()
            event.unhandledCallback = self.onUnhandledMessage
            setattr(self, eventName, event)

        self.init()
        self.onSetOptions(**options)

    def setOptions(self, **options):
        self.onSetOptions(**options)

    def init(self):
        """A hook for subclasses to add initialization in the proper sequence"""
        pass

    def getMsgHandlerName(self, messageClass):
        return "on%s" % messageClass.__name__

    def onSetOptions(self, **options):
        self.options.update(options)

        if 'eventLoop' in options.keys():
            self.eventLoop = options['eventLoop']
        else:
            from BZFlag import Event
            self.eventLoop = Event.EventLoop()

    def getSupportedOptions(self):
        return self.options.keys()

    def run(self):
        self.eventLoop.run()

    def handleMessage(self, socket, eventLoop):
        """This is a callback used to handle incoming socket
           data when we're expecting a message.
           """
        # This can return None if part of the mesasge but not the whole
        # thing is available. The rest of the message will be rebuffered,
        # so we'll read the whole thing next time this is called.
        msg = socket.readMessage(self.incoming)
        if msg:
            msg.socket = socket
            msg.eventLoop = eventLoop
            handler = getattr(self, self.getMsgHandlerName(msg.__class__))
            if self.onAnyMessage(msg):
                return
            handler(msg)

    def onUnhandledMessage(self, msg):
        raise Errors.ProtocolWarning("Unhandled message %s" % msg.__class__.__name__)

### The End ###

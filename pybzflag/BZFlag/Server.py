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
from BZFlag import Network, Protocol


class BaseServer:
    """On top of the basic message processing provided by Network.Endpoint,
       this class manages connections from clients.
       """

    outgoing = Protocol.FromServer
    incoming = Protocol.ToServer
    
    def init(self):
        Network.Endpoint.init(self)
        self.clients = {}
        self.options.update({
            'interface': None,
            })
        Event.attach(self, 'onConnect', 'onDisconnect')

        def setOptions(**options):
            if 'interface' in options.keys():
                self.bind(options['interface'])
        self.onSetOptions.observe(setOptions)

    def bind(self, interface):


### The End ###

""" BZFlag.Meta

Package for the BZFlag metaserver protocol. The metaserver is a centralized
server that stores information about other servers.
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
from BZFlag import Network
import urlparse


# The file at this URL contains the URL for the BZFlag metaserver
metaMetaURL = "http://bzflag.org/list-server.txt"


# The URL of the metaserver, which will be retrieved by getMetaserverURL()
metaURL = None

def getMetaserverURL(eventLoop, callback):
    """Contact the meta-meta-server to find the URL for the metaserver.
       call the provided callback with the URL once we find it.
       """
    global metaMetaURL, metaURL
    if metaURL:
        callback(metaURL)
    else:
        # Stick together an HTTP request, with responseHandler called when it comes back.
        def responseHandler(response):
            metaURL = response.strip().split("\n")[-1].strip()
            callback(metaURL)
        parsed = urlparse.urlparse(metaMetaURL)
        Network.asyncRequest(eventLoop, parsed[1], 80,
                             "GET %s HTTP/1.1\nHost: %s\nUser-Agent: %s/%s\n\n" %
                             (parsed[2], parsed[1], BZFlag.name, BZFlag.version),
                             responseHandler)

### The End ###

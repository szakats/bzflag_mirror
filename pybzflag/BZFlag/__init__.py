""" BZFlag

This is a Python package providing an interface to all network
protocols used by BZFlag.
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

# The version of the BZFlag protocol this should be compatible with
protocolVersion = "BZFS109a"

# Information about this implementation
name    = "PyBZFlag"
version = "cvs-dev"

about = "%s - code version %s, protocol version %s" % \
        (name, version, protocolVersion)

serverWelcomeMessage = "Welcome to the %s Server (code/%s protocol/%s)" % \
                       (name, version, protocolVersion)

### The End ###

""" BZFlag.UI.Drawable.Sky

A class to draw a sky cube at various times of day
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
from DisplayList import *
from OpenGL.GL import *

class Sky(DisplayList):
    def __init__(self):
        DisplayList.__init__(self)
        self.time = 'sunrise'

        self.color = {}
        self.color['sunrise'] = []
        self.color['sunrise'].append((0.04, 0.04, 0.08, 1))
        self.color['sunrise'].append((0.04, 0.04, 0.08, 1))
        self.color['sunrise'].append((0.04, 0.04, 0.08, 1))
        self.color['sunrise'].append((0.04, 0.04, 0.08, 1))
        self.color['sunrise'].append((0.47, 0.12, 0.08, 1))
        self.color['sunrise'].append((0.04, 0.04, 0.08, 1))
	self.color['sunset'] = self.color['sunrise']

        self.color['night'] = []
        self.color['night'].append((0.04, 0.04, 0.08, 1))
        self.color['night'].append((0.04, 0.04, 0.08, 1))
        self.color['night'].append((0.04, 0.04, 0.08, 1))
        self.color['night'].append((0.04, 0.04, 0.08, 1))
        self.color['night'].append((0.04, 0.04, 0.08, 1))
        self.color['night'].append((0.04, 0.04, 0.08, 1))

        self.color['twilight'] = []
        self.color['twilight'].append((0.04, 0.04, 0.08, 1))
        self.color['twilight'].append((0.04, 0.04, 0.08, 1))
        self.color['twilight'].append((0.04, 0.04, 0.08, 1))
        self.color['twilight'].append((0.04, 0.04, 0.08, 1))
        self.color['twilight'].append((0.30, 0.12, 0.08, 1))
        self.color['twilight'].append((0.04, 0.04, 0.08, 1))

	self.color['morning'] = []
        self.color['morning'].append((0.25, 0.55, 0.86, 1))
        self.color['morning'].append((0.43, 0.75, 0.95, 1))
        self.color['morning'].append((0.43, 0.75, 0.95, 1))
        self.color['morning'].append((0.43, 0.75, 0.95, 1))
        self.color['morning'].append((0.43, 0.75, 0.95, 1))
        self.color['morning'].append((0.25, 0.55, 0.86, 1))
	self.color['evening'] = self.color['morning']

    def drawToList(self, rstate):
        pass

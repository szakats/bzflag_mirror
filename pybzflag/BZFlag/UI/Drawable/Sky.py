""" BZFlag.UI.Drawable.Sky

A class to draw a sky cube at various times of day
"""
#
# Python BZFlag Package
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


colorScheme = {
    'sunrise': ( (0.04, 0.04, 0.08, 1),
                 (0.04, 0.04, 0.08, 1),
                 (0.04, 0.04, 0.08, 1),
                 (0.04, 0.04, 0.08, 1),
                 (0.47, 0.12, 0.08, 1),
                 (0.04, 0.04, 0.08, 1) ),

    'sunset': ( (0.04, 0.04, 0.08, 1),
                (0.04, 0.04, 0.08, 1),
                (0.04, 0.04, 0.08, 1),
                (0.04, 0.04, 0.08, 1),
                (0.47, 0.12, 0.08, 1),
                (0.04, 0.04, 0.08, 1) ),

    'night': ( (0.04, 0.04, 0.08, 1),
               (0.04, 0.04, 0.08, 1),
               (0.04, 0.04, 0.08, 1),
               (0.04, 0.04, 0.08, 1),
               (0.04, 0.04, 0.08, 1),
               (0.04, 0.04, 0.08, 1) ),

    'twilight': ( (0.04, 0.04, 0.08, 1),
                  (0.04, 0.04, 0.08, 1),
                  (0.04, 0.04, 0.08, 1),
                  (0.04, 0.04, 0.08, 1),
                  (0.30, 0.12, 0.08, 1),
                  (0.04, 0.04, 0.08, 1) ),

    'morning': ( (0.25, 0.55, 0.86, 1),
                 (0.43, 0.75, 0.95, 1),
                 (0.43, 0.75, 0.95, 1),
                 (0.43, 0.75, 0.95, 1),
                 (0.43, 0.75, 0.95, 1),
                 (0.25, 0.55, 0.86, 1), ),

    'evening': ( (0.25, 0.55, 0.86, 1),
                 (0.43, 0.75, 0.95, 1),
                 (0.43, 0.75, 0.95, 1),
                 (0.43, 0.75, 0.95, 1),
                 (0.43, 0.75, 0.95, 1),
                 (0.25, 0.55, 0.86, 1), ),
    }
        
        
class Sky(DisplayList):
    def __init__(self):
        DisplayList.__init__(self)
        self.time = 'sunrise'

    def drawToList(self, rstate):
        pass

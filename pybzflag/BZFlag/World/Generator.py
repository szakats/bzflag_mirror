""" BZFlag.World.Generator

Implements factory functions for automatically generating various worlds
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

from BZFlag.World.World import World
from BZFlag.World import Scale
from BZFlag.Protocol import WorldObjects
from random import random
import math

__all__ = ['Empty', 'Random']


def Empty():
    w = World()
    w.erase()
    w.storeSkeletonHeader()
    w.storeSkeletonFooter()    
    w.postprocess()
    return w


def Random(randomHeights=0):
    """This is the same random algorithm that bzfs.cxx uses by default"""
    w = World()
    w.erase()
    w.lifetime = 'temporary'
    w.storeSkeletonHeader()

    def addRandom(cls, height, base):
        h = height
        num = int((0.5 + 0.7 * random()) * Scale.CitySize * Scale.CitySize)
        for i in xrange(num):
            if randomHeights:
                h = height * (2.0 * (random() + 0.5))
            w.storeBlock(cls(
                center = [Scale.WorldSize * (random() - 0.5),
                          Scale.WorldSize * (random() - 0.5),
                          0],
                angle = 2.0 * math.pi * random(),
                size = [base, base, h]))
    
    addRandom(WorldObjects.Box, Scale.BoxHeight, Scale.BoxBase)
    addRandom(WorldObjects.Pyramid, Scale.PyrHeight, Scale.PyrBase)
    
    w.storeSkeletonFooter()    
    w.postprocess()
    return w

### The End ###

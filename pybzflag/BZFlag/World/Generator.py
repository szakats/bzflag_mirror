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
from BZFlag import Util, Errors

__all__ = ['Empty', 'Random', 'Text', 'Binary', 'Heightmap']


def Empty(size=Scale.WorldSize,
          wallHeight=Scale.WallHeight,
          ):
    w = World()
    w.erase()
    w.storeSkeletonHeader(size=size, wallHeight=wallHeight)
    w.storeSkeletonFooter()    
    w.postprocess()
    return w


def Random(size          = Scale.WorldSize,
           wallHeight    = Scale.WallHeight,
           randomHeights = False,
           pyrBase       = 4.0 * Scale.TankHeight,
           pyrHeight     = 5.0 * Scale.TankHeight,
           boxBase       = 5.0 * Scale.TankLength,
           boxHeight     = 6.0 * Scale.MuzzleHeight,
           citySize      = 5,
           ):
    """The defaults here produce the same results as bzfs.cxx's builtin
       random world generator, but it's a bit more flexible.
       """
    from random import random
    import math

    w = World()
    w.erase()
    w.lifetime = 'temporary'
    w.storeSkeletonHeader(size=size, wallHeight=wallHeight)

    def addRandom(cls, height, base):
        h = height
        num = int((0.5 + 0.7 * random()) * citySize * citySize)
        for i in xrange(num):
            if randomHeights:
                h = height * (2.0 * (random() + 0.5))
            w.storeBlock(cls(
                center = [size * (random() - 0.5),
                          size * (random() - 0.5),
                          0],
                angle = 2.0 * math.pi * random(),
                size = [base, base, h]))
    
    addRandom(WorldObjects.Box, boxHeight, boxBase)
    addRandom(WorldObjects.Pyramid, pyrHeight, pyrBase)
    
    w.storeSkeletonFooter()    
    w.postprocess()
    return w


def Text(name,
         size       = Scale.WorldSize,
         wallHeight = Scale.WallHeight,
         ):
    """Load a world from a text file. 'name' can be a file name,
       URI name, or a file-like object.
       """
    from xreadlines import xreadlines
    import re
    
    f = Util.autoFile(name)
    w = World()
    section = None
    sectionDict = Util.getSubclassDict(WorldObjects, WorldObjects.WorldObject,
                                       'textName', 'textSectionDict')
        
    # Start with a fresh map, and add objects that
    # are implied but not specified in the text map format.
    w.erase()
    w.storeSkeletonHeader(size, wallHeight)
        
    for line in xreadlines(f):
        # If this is a kludge used by map editors to store extra
        # attributes. Don't process any comments on the line.
        if not line.startswith("#!"):
            line = re.sub("#.*", "", line)
        line = line.strip()
        if line:
            if section:
                # We're inside a section. Accumulate lines until 'end'
                sectionLines.append(line)
                if line == 'end':
                    # Done with this section, process it.
                    try:
                        cls = sectionDict[section]
                    except KeyError:
                        raise Errors.ProtocolError(
                            "World file contains unknown section type '%s'" % section)
                    inst = cls()
                    inst.textRead(sectionLines)
                    w.storeBlock(inst)
                    section = None
            elif not line.startswith("#!"):
                # We're beginning a section
                section = line
                sectionLines = []

    w.storeSkeletonFooter()
    w.postprocess()
    return w


def Binary(name):
    """Load a world from a binary file. 'name' can be a file name,
       URI name, or a file-like object.
       """
    from BZFlag.Protocol import Common
    
    blockDict = Common.getMessageDict(WorldObjects)
    f = Util.autoFile(name)
    w = World()
    w.erase()
    while 1:
        # Read the block header
        header = WorldObjects.BlockHeader()
        packedHeader = f.read(header.getSize())
        if len(packedHeader) < header.getSize():
            raise Errors.ProtocolError("Premature end of binary world data")
        header.unmarshall(packedHeader)

        # Look up the block type and instantiate it
        try:
            block = blockDict[header.id]()
        except KeyError:
            raise Errors.ProtocolError(
                "Unknown block type 0x%04X in binary world data" % header.id)

        # Read the block body
        packedBody = f.read(block.getSize() - len(packedHeader))
        if len(packedBody) < (block.getSize() - len(packedHeader)):
            raise Errors.ProtocolError("Incomplete block in binary world data")
        block.unmarshall(packedHeader + packedBody)
        w.storeBlock(block)

        # We're done if this was the EndOfData block
        if isinstance(block, WorldObjects.EndOfData):
            break
    w.postprocess()
    return w


def Heightmap(name,
              size          = Scale.WorldSize,
              wallHeight    = Scale.WallHeight,
              boxHeight     = 6.0 * Scale.MuzzleHeight,
              ):
    """Uses the given image file to make a heightmapped world.
       This is very silly and nobody should use this except
       as an example and a proof-of-concept.
       Requires PIL.
       """
    import Image
    f = Util.autoFile(name, "rb")
    img = Image.open(f).convert("L")   # Convert to grayscale
    w = World()
    w.erase()
    w.storeSkeletonHeader(size=size, wallHeight=wallHeight)

    # For now just output a box per nonzero pixel. If I expected
    # anyone to actually use this I'd make it combine rectangular
    # regions of the same pixel value into larger boxes.
    for y in xrange(img.size[1]):
        for x in xrange(img.size[0]):
            p = img.getpixel((x,y))
            if p:
                w.storeBlock(WorldObjects.Box(
                    center = ((float(x) / img.size[0] - 0.5) * size,
                              (float(y) / img.size[1] - 0.5) * size,
                              0),
                    size   = (size / img.size[0],
                              size / img.size[1],
                              p / 255.0 * boxHeight),
                    angle  = 0))
    
    w.storeSkeletonFooter()    
    w.postprocess()
    f.close()
    return w



### The End ###

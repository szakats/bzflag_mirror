""" BZFlag.World

Implementation of BZFlag's world database. This includes loading
and saving worlds in binary and text formats.
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

from BZFlag.Protocol import WorldObjects, Common
from BZFlag import Errors
import os


class Scene:
    """Abstract base class for a scene manager. This class provides
       a way to store scene objects and later select objects based
       on geometric criteria, for example frustum culling.
       """
    def add(self, block):
        pass

    def clip(self, filter, action):
        """Filter blocks through the provided filter function, passing
           them to the provided action function if they pass. The
           implementation is free to assume that if the filter fails
           for any particular volume, it will also fail for any objects
           contained completely within that volume.
           """
        pass

    def filter(self, filter, action):
        """Like clip, but doesn't assume the filter is geometric in
           nature so it is forced to test all scene nodes.
           """
        pass

    def __iter__(self):
        """Scene subclasses must support python's iterator protocol"""
        pass


class SceneList:
    """Implementation of Scene using a flat list"""
    def __init__(self):
        self.list = []
        self.clip = self.filter

    def add(self, block):
        self.list.append(block)

    def filter(self, filter, action):
        for item in self.list:
            if filter(item):
                action(item)

    def __iter__(self):
        scene = self
        class Iterator:
            def __init__(self):
                self.index = 0
            def __iter__(self):
                return self
            def next(self):
                if self.index >= len(scene.list):
                    raise StopIteration
                item = scene.list[self.index]
                self.index += 1
                return item
        return Iterator()


class TeleporterSide:
    """Represent one side of a teleporter. This is used to link teleporters together"""
    def __init__(self, teleporter, side):
        self.teleporter = teleporter
        self.side = side

    def link(self, to):
        if not hasattr(self.teleporter, 'links'):
            self.teleporter.links = [None, None]
        self.teleporter.links[self.side] = to


class World:
    """Abstraction for a BZFlag world. Currently this can only be created
       from binary worlds downloaded from the server, but eventually this
       will need to be able to read textual bzflag world files as well.

       The data structure used to store the world's contents in a form efficient
       for in-game use is separated into a Scene class. You can tell this class
       which Scene implementation to use in the constructor. By default it uses
       a flat list, but if this is being used for a 3D client it might be better
       to implement an Octree.
       """
    def __init__(self, sceneClass=SceneList):
        self.sceneClass = sceneClass
        self.erase()

    def loadBinary(self, f):
        """Load a binary world from the supplied file-like object"""
        blockDict = Common.getMessageDict(WorldObjects)
        self.erase()
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
            if isinstance(block, WorldObjects.EndOfData):
                break

            # Read the block body
            packedBody = f.read(block.getSize() - len(packedHeader))
            if len(packedBody) < (block.getSize() - len(packedHeader)):
                raise Errors.ProtocolError("Incomplete block in binary world data")
            block.unmarshall(packedHeader + packedBody)
            self.storeBlock(block)
        self.postprocess()

    def saveText(self, f):
        """Save a text world to the supplied file-like object"""
        for block in self.blocks:
            if isinstance(block, WorldObjects.WorldObject):
                block.textWrite(f)

    def erase(self):
        """Reset all internal structures, prepare to load a world"""
        self.blocks = []
        self.teleporters = []
        self.teleporterLinks = []
        self.scene = self.sceneClass()
        self.gameStyle = None

    def storeBlock(self, block):
        """Store one block class. This will be called while loading a world,
           between erase() and postprocess().
           """
        # File this block in the appropriate lists
        self.blocks.append(block)
        if isinstance(block, WorldObjects.Style):
            self.gameStyle = block
            # Use a 2D size to make it easy to support non-square worlds in the future
            self.size = (block.worldSize, block.worldSize)
        if hasattr(block, 'center'):
            self.scene.add(block)
        if isinstance(block, WorldObjects.TeleporterLink):
            self.teleporterLinks.append(block)
        if isinstance(block, WorldObjects.Teleporter):
            self.teleporters.append(block)

    def postprocess(self):
        """Performs any checks or calculations necessary
           after a world has completed loading.
           """
        # Link teleporters to each other.
        # Note that this creates circular links, so the world will not
        # be automatically garbage collected! This isn't a problem yet,
        # since only one world should be loaded.
        for link in self.teleporterLinks:
            fromSide = TeleporterSide(self.teleporters[link.fromSide >> 1], link.fromSide & 1)
            toSide = TeleporterSide(self.teleporters[link.toSide >> 1], link.toSide & 1)
            fromSide.link(toSide)


class Cache:
    """Cache worlds on disk according to a server-generated hash,
       so we don't always have to send them over the wire.
       """
    def __init__(self, path="~/.bzflag-cache"):
        self.path = os.path.expanduser(path)
        try:
            os.makedirs(self.path)
        except OSError:
            # Path could already exist or it could be unreachable. We don't care.
            pass

    def getFilename(self, hash):
        return os.path.join(self.path, hash) + ".bwc"

    def hasWorld(self, hash):
        try:
            return os.path.isfile(self.getFilename(hash))
        except OSError:
            return 0

    def storeWorld(self, hash, data):
        try:
            f = open(self.getFilename(hash), "wb")
            f.write(data)
            f.close()
        except OSError:
            # In case the file was corrupted (if the disk was full, for example)
            # try to remove it before giving up completely.
            try:
                os.remove(self.getFilename(hash))
            except OSError:
                pass

    def openWorld(self, hash):
        # Don't catch OSError here. If the cache wasn't valid we
        # should have stopped after hasWorld() returned zero.
        # At this point, we're committed to using the cache.
        return open(self.getFilename(hash), "rb")

### The End ###

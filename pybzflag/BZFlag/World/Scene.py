""" BZFlag.World.Scene

Implements the Scene class used as a backend for storing and searching
the contents of a world. The Scene is an abstract base class that can
be implemented as a flag list, an Octree, Quadtree, or just about
anything.
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

class Scene:
    """Abstract base class for a scene manager. This class provides
       a way to store scene objects and later provide iterators for
       sorting or selecting geometry based on different criteria

       Right now this only supports iterating through all available
       objects. New iterators will be added as needed, but it is
       expected that we will need iterators for finding objects that
       intersect with geometry, and for sorting objects by Z order.
       """
    def add(self, block):
        pass

    def __iter__(self):
        """Scene subclasses must support python's iterator protocol
           for iterating through all available objects.
           """
        pass


class SceneList:
    """Implementation of Scene using a flat list"""
    def __init__(self):
        self.list = []

    def add(self, block):
        self.list.append(block)

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

### The End ###

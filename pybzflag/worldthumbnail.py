#!/usr/bin/env python
#
# usage: worldthumbnail.py worldName thumbFile
#   Saves a thumbnail image of the given world
#
from BZFlag import World
from BZFlag.UI import OverheadView
import sys

world = World.load(sys.argv[1])
img = OverheadView.worldToImage(world, (500,500), 2)
img.save(sys.argv[2])

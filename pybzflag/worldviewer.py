#!/usr/bin/env python
#
# Simple viewer for BZFlag world files
#
from BZFlag.World import World

w = World()
w.loadText(open("/home/micah/bzflag/worlds/RunRabbitRun.bzw"))
w.saveBinary(open("foo.bin", "wb"))


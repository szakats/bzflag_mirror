#!/usr/bin/env python
#
# Simple viewer for BZFlag world files. Give a .bzw file on the command line.
#
from BZFlag import World, Game
from BZFlag.UI import OverheadView
from BZFlag.Event import EventLoop
import sys

game = Game.Game()
game.world = World.load(sys.argv[1])
loop = EventLoop()
OverheadView.attach(game, loop)
loop.run()



#!/usr/bin/env python
#
# Simple viewer for BZFlag world files. Give a .bzw file on the command line.
#
from BZFlag.Game import Game
from BZFlag.UI import OverheadView
from BZFlag.Event import EventLoop
import sys

game = Game()
loop = EventLoop()
OverheadView.attach(game, loop)
game.loadWorld(sys.argv[1])
loop.run()



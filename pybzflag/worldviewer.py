#!/usr/bin/env python
#
# Simple viewer for BZFlag world files
#
from BZFlag.World import World
from BZFlag.Game import Game
from BZFlag.UI.OverheadView import OverheadView

game = Game()
game.world.loadText(open("/home/micah/bzflag/worlds/RunRabbitRun.bzw"))




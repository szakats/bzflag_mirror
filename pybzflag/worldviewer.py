#!/usr/bin/env python
#
# Simple viewer for BZFlag world files. Give any valid world name on the
# command line. A world name of 'help' prints information about the name format.
#
from BZFlag import World, Game, UI, CommandLine
from BZFlag.Event import EventLoop
import sys

ui = CommandLine.Parser(UI.Any, ui = 'overhead').parse()
game = Game.Game()
loop = EventLoop()
game.world.clone(World.load(ui.cmdLineArgs[0]))
ui.attach(game, loop)
loop.run()



#!/usr/bin/env python
#
# Simple viewer for BZFlag world files. Give any valid world name on the
# command line. A world name of 'help' prints information about the name format.
#
from BZFlag import World, Game, UI, CommandLine, Animated
from BZFlag.Event import EventLoop
import sys

ui = CommandLine.Parser(UI.Any, ui = 'overhead').parse()

try:
    fileName = ui.cmdLineArgs[0]
except IndexError:
    print "A world name must be specified on the command line."
    print "Try 'help' for information about the name format."
    sys.exit(0)

game = Game.Game()
loop = EventLoop()
game.world.clone(World.load(ui.cmdLineArgs[0]))

# Create the UI, add a frame rate indicator
viewport = ui.attach(game, loop)
Animated.FrequencyCounter(viewport.onFinishFrame,
                          lambda hz: "FHz: %.3f (target %.3f)" % (hz, viewport.targetFrameRate))

loop.run()



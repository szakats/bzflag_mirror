#!/usr/bin/env python
from BZFlag import CommandLine
from BZFlag.UI import OverheadView

client = CommandLine.client(server     = "localhost",
                            callSign   = "@PyObserver")

# Stick some instrumentation in select events
client.onConnect.trace("Connected.")
client.onStartWorldDownload.trace("Downloading world...")
client.onEnterGame.trace("Entered the game.")
client.game.onLoadWorld.trace("World loaded.")
client.game.onAddPlayer.trace("Added player %(2)s")
client.game.onRemovePlayer.trace("Removed player %(2)s")

# Show messages
def message(msg):
    print "<%s> %s" % (msg.fromId, msg.message)
client.onMsgMessage.observe(message)

# Show an overhead view of the world
OverheadView.attach(client.game, client.eventLoop)
client.run()

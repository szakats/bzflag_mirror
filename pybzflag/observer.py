#!/usr/bin/env python
from BZFlag import CommandLine

client = CommandLine.client(server     = "localhost",
                            callSign   = "Super Observer Duckie",
                            playerType = "observer")

# Stick some instrumentation in select events
client.onConnect.trace("Connected.")
client.onStartWorldDownload.trace("Downloading world...")
client.onLoadWorld.trace("World loaded.")
client.onEnterGame.trace("Entered the game.")
client.game.onAddPlayer.trace("Added player %(2)s")
client.game.onRemovePlayer.trace("Removed player %(2)s")

# Show messages
def message(msg):
    print "<%s> %s" % (msg.fromId, msg.message)
client.onMsgMessage.observe(message)

def worldMunge():
    for object in client.game.world.scene:
        print object.__class__.__name__
client.onLoadWorld.observe(worldMunge)

client.run()

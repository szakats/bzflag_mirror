#!/usr/bin/env python
from BZFlag import CommandLine
import readline

client = CommandLine.client(server     = "localhost",
                            callSign   = "@Jabberwocky")

# Show connection events
client.onConnect.trace("--- Connected to %s" % client.options['server'])
client.onStartWorldDownload.trace("--- Downloading world...")
client.onEnterGame.trace("--- Entered the game.")
client.game.onLoadWorld.trace("--- World loaded.")
client.game.onAddPlayer.trace("--> Joined : %(2)s")
client.game.onRemovePlayer.trace("<-- Parted : %(2)s")

# Show incoming messages
def message(msg):
    sender = msg.fromId
    if type(sender) == int:
        # This is from a player, look up the callsign
        sender = "<%s>" % client.game.players[sender].identity.callSign
    else:
        # This is from a special 'player', like the server
        sender = "[%s]" % sender
    print "%s %s" % (sender, msg.message)
client.onMsgMessage.observe(message)

def sayHi():
    client.sendMessage("Hello")
client.onEnterGame.observe(sayHi)

client.run()

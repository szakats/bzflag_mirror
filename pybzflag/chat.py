#!/usr/bin/env python
from BZFlag import CommandLine
from threading import *
import sys

argParser = CommandLine.ClientParser(server   = "localhost",
                                     callSign = "@PyChat")
argParser.add_option("-q", "--quiet", action="store_true", dest="quiet",
                     help="Disables the output of connection status messages and protocol warnings.")
argParser.add_option("-i", "--silent", action="store_true", dest="silent",
                     help="Disables all output.")
client = argParser.parse()

# Disable warnings by default, enable them below if we're not being quiet
client.eventLoop.showNonfatalExceptions = 0

if not client.cmdLineValues['silent']:
    if not client.cmdLineValues['quiet']:
        # Show connection events
        client.onConnect.trace("--- Connected to %s" % client.options['server'])
        client.onStartWorldDownload.trace("--- Downloading world...")
        client.onEnterGame.trace("--- Entered the game.")
        client.game.onLoadWorld.trace("--- World loaded.")
        client.game.onAddPlayer.trace("--> Joined : %(2)s")
        client.game.onRemovePlayer.trace("<-- Parted : %(2)s")
        client.eventLoop.showNonfatalExceptions = 1

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

# Thread to read keyboard input and send messages.
# It would generally be better to do this by making stdin nonblocking,
# but there's no portable way to do this in pyhton. Threads are slightly
# more portable :)
class ChatThread(Thread):
    def run(self):
        while client.eventLoop.running:
            line = sys.stdin.readline()
            # If this is end of file, exit or we'll spin in an infinite loop
            if not line:
                client.eventLoop.stop()
            # Only send non-blank lines
            line = line.strip()
            if line:
                client.sendMessage(line)
client.onEnterGame.observe(ChatThread().start)

client.run()

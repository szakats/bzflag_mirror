#!/usr/bin/env python

from BZFlag import Event, Meta

loop = Event.EventLoop()

def callback(url):
    print url
    loop.stop()

Meta.getMetaserverURL(loop, callback)

loop.run()

#import BZFlag.ListServer
#for server in BZFlag.ListServer.getDefault().filteredList():
#    print server.info()


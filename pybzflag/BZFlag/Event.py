""" BZFlag.Event

Event processing utilities, including the default EventLoop class
and an Event class that can transparently convert member functions
into observable and traceable events.
"""
#
# Python BZFlag Protocol Package
# Copyright (C) 2003 Micah Dowty <micahjd@users.sourceforge.net>
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

import select


class Event:
    """An event that can be observed and triggered.
       This event can be called, and all observers will be
       called with the same arguments. Simple and effective :)
       The observers are called in an undefined order.

       The event can be constructed with a list of initial observers.
       This makes it easy to transparently make a member function
       an event with a line like:
         self.foo = Event.Event(self.foo)

       See the attach() function below, it provides a way to
       set up Event wrappers on member functions in bulk.
       """
    def __init__(self, *args):
        self.clients = {}
        for arg in args:
            self.observe(arg)
        self.unhandledCallback = None

    def observe(self, callback):
        self.clients[callback] = 1

    def unobserve(self, callback):
        del self.clients[callback]

    def __call__(self, *args, **kw):
        if self.clients:
            for client in self.clients.keys():
                r = client(*args, **kw)
                # Allow the client to abort
                if r:
                    return r
        else:
            # No handlers- can we call the unhandled event callback?
            if self.unhandledCallback:
                self.unhandledCallback(*args, **kw)

    def trace(self, fmt):
        """A debugging aid- whenever this event is triggered,
           print a line using the supplied format string to
           represent the call parameters.
           """
        def traceCallback(*args, **kw):
            # Make a dictionary with both keyword args and normal
            # args, representing normal args by their place in the
            # argument list, starting with 1.
            index = 1
            for arg in args:
                kw[str(index)] = arg
                index += 1
            print fmt % kw
        self.observe(traceCallback)


def attach(cls, *args):
    """A convenience function for setting up several transparent
       Event instances. Pass this your class instance and the names
       of all the functions you'd like turned into events.
       If any of the names specified don't exist yet, they are
       still set to a new Event instance, but no callback will
       be associated with it yet.
       """
    for arg in args:
        if hasattr(cls, arg):
            setattr(cls, arg, Event(getattr(cls, arg)))
        else:
            setattr(cls, arg, Event())


class EventLoop:
    def __init__(self):
        # No polling by default. This can be changed to a duration
        # between polls, or to zero to poll continuously.
        self.pollTime = None
        attach(self, 'onPoll', 'onNonfatalException')
        self.sockets = []
        self.showNonfatalExceptions = 1

    def registerSocket(self, socket):
        self.sockets.append(socket)

    def unregisterSocket(self, socket):
        self.sockets.remove(socket)

    def run(self):
        self.running = 1
        try:
            # Make a dictionary for quickly detecting which socket has activity
            selectDict = {}
            for socket in self.sockets:
                selectable = socket.getSelectable()
                selectDict[selectable] = socket
            selectables = selectDict.keys()

            while self.running:
                try:
                    (iwtd, owtd, ewtd) = select.select(selectables, [], [], self.pollTime)
                except select.error:
                    raise Errors.ConnectionLost()
                readyList = iwtd + owtd + ewtd
                for ready in readyList:
                    try:
                        selectDict[ready].poll(self)
                    except Errors.NonfatalException:
                        self.onNonfatalException(sys.exc_info())
                self.onPoll()
        finally:
            self.running = 0

    def stop(self):
        self.running = 0

    def onNonfatalException(self, info):
        if self.showNonfatalExceptions:
            print "*** %s : %s" % (info[1].__class__.__name__, info[1])

### The End ###

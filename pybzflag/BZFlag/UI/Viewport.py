""" BZFlag.UI.Viewport

A Viewport in this context is an area that can be drawn to and
provides event handling. The idea is that views associate with
viewports for event handling and some of their context management
rather than directly with the underlying graphics library, so that
it's easy to show multiple views in the same frame.

For every underlying graphics library supported, this module
provides a class that manages event handling for that library.
This includes redraw events, user-supplied events, and connecting
the UI library with the event loop. In turn, the viewport provides
events that views and controllers can hook on to.

The reasoning behind having events multiplex through the view as
well as rendering is so that mouse events can be made relative to
the view, keyboard events can have focus handled, etc. depending
on the needs of a particular view multiplexer class.
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

from BZFlag import Event


class Viewport:
    """Abstract base class for a viewport. Defines events and methods
       that all viewports must support.
       """
    def __init__(self, eventLoop):
        self.eventLoop = eventLoop
        Event.attach(self, "onInit")

    def setCaption(self, title):
        """Set the window caption on the viewport, if applicable"""
        pass


class PygameView(Viewport):
    """A basic viewport for 2D pygame rendering. This handles all pygame
       events, and is subclassed below for 3D rendering via pygame.
       """
    def __init__(self, eventLoop, size=(640,480), targetFrameRate=60):
        Viewport.__init__(self, eventLoop)
        Event.attach(self, 'onFrame', 'onSetupFrame', 'onDrawFrame', 'onFinishFrame', 'onResize')
        import pygame
        pygame.init()
        self.display = pygame.display
        self.resize(size)
        self.init()
        self.onInit()
        self.frameTimer = None
        self.setTargetFrameRate(targetFrameRate)

    def init(self):
        """Hook for subclasses to add more initialization code at the proper place"""
        pass

    def getModeFlags(self):
        """A hook for subclasses to override the mode flags used to initialize pygame"""
        import pygame
        return pygame.DOUBLEBUF | pygame.RESIZABLE 

    def resize(self, size):
        self.screen = self.display.set_mode(size, self.getModeFlags())
        self.size = size
        self.onResize(size)

    def setCaption(self, title):
        import pygame
        pygame.display.set_caption(title)
        
    def setTargetFrameRate(self, rate):
        """Set the target frame rate. This is how often the main loop will
           try to call us for rendering, using a PeriodicTimer.
           """
        if self.frameTimer:
            self.eventLoop.remove(self.frameTimer)
        self.frameTimer = Event.PeriodicTimer(1.0 / rate, self.onFrame)
        self.eventLoop.add(self.frameTimer)

    def onFrame(self):
        """This event is called by the PeriodicTimer to poll for events and
           render a frame. The task is divided into events for setting up the
           rendering, actually performing the rendering, and finishing the frame.
           """
        import pygame
        for event in pygame.event.get():
            self.onEvent(event)

        self.onSetupFrame()
        self.onDrawFrame()
        self.onFinishFrame()

    def onFinishFrame(self):
        """Default onFinishFrame handler that flips the pygame buffers"""
        self.display.flip()
        
    def onEvent(self, event):
        print "Event %s" % event.type

### The End ###

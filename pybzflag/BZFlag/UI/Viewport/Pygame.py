""" BZFlag.UI.Viewport.Pygame

Viewport implementation using Pygame. On top of the abstract Viewport,
this provides frame rate control, surface initialization, event dispatch.
"""
#
# Python BZFlag Package
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

from BZFlag import Event, Animated
import copy, pygame
from Base import Viewport


class PygameViewport(Viewport):
    """A basic viewport for 2D pygame rendering. This handles all pygame
       events, and is subclassed below for 3D rendering via pygame.
       """
    def __init__(self, eventLoop, size=(640,480), targetFrameRate=100, resizable=True):
        Viewport.__init__(self, eventLoop)

        # Add event handlers for all the pygame events we can find
        for i in xrange(100):
            name = pygame.event.event_name(i)
            if name != "Unknown":
                Event.attach(self, 'on' + name)

        pygame.display.init()
        self.display = pygame.display
        self.resizable = resizable
        self.resize(size)
        self.init()
        self.frameTimer = None
        self.setTargetFrameRate(targetFrameRate)

    def init(self):
        """Hook for subclasses to add more initialization code at the proper place"""
        pass

    def getModeFlags(self):
        """A hook for subclasses to override the mode flags used to initialize pygame"""
        flags = pygame.DOUBLEBUF
        if self.resizable:
            flags |= pygame.RESIZABLE
        return flags

    def resize(self, size):
        self.screen = self.display.set_mode(size, self.getModeFlags())
        self.size = size
        self.onResize()

    def setCaption(self, title):
        pygame.display.set_caption(title)

    def setTargetFrameRate(self, rate):
        """Set the target frame rate. This is how often the main loop will
           try to call us for rendering, using a PeriodicTimer.
           A rate of 'None' will disable continuous rendering.
           """
        self.targetFrameRate = rate
        if self.frameTimer:
            self.eventLoop.remove(self.frameTimer)
        if rate:
            self.frameTimer = Event.PeriodicTimer(1.0 / rate, self.onFrame)
            self.eventLoop.add(self.frameTimer)
        else:
            self.frameTimer = None

    def onFrame(self):
        """This event is called by the PeriodicTimer to poll for events and
           render a frame. The task is divided into events for setting up the
           rendering, actually performing the rendering, and finishing the frame.
           """
        # Poll for events
        for event in pygame.event.get():
            self.onEvent(event)

        # Update the global timestep for this frame
        Animated.currentTimeMaster.update()

        # Call all of our rendering callbacks. This includes subviewports.
        self.render()

    def onFinishFrame(self):
        """Default onFinishFrame handler that flips the pygame buffers"""
        self.display.flip()

    def onEvent(self, event):
        getattr(self, 'on' + pygame.event.event_name(event.type))(event)

    def onQuit(self, event):
        self.eventLoop.stop()

    def onVideoResize(self, event):
        self.resize(event.size)

    def region(self, rect):
        """Return a class that represents a rectangular subsection of this viewport.
           This performs a shallow copy on ourselves, then subsurfaces its screen.
           """
        if callable(rect):
            rect = rect

        sub = copy.copy(self)
        sub.screen = self.secreen.subsurface(rect)
        sub.size = rect[2:]
        sub.parent = self

        # Ignore the caption on sub-viewports
        sub.setCaption = lambda title: None
        return sub

### The End ###

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
import copy


class Viewport:
    """Abstract base class for a viewport. Defines events and methods
       that all viewports must support.
       """
    def __init__(self, eventLoop):
        self.eventLoop = eventLoop

    def setCaption(self, title):
        """Set the window caption on the viewport, if applicable"""
        pass


class PygameViewport(Viewport):
    """A basic viewport for 2D pygame rendering. This handles all pygame
       events, and is subclassed below for 3D rendering via pygame.
       """
    def __init__(self, eventLoop, size=(640,480), targetFrameRate=60):
        import pygame
        Viewport.__init__(self, eventLoop)

        # Add event handlers for all our builtin events, and all the pygame events we can find
        Event.attach(self, 'onFrame', 'onSetupFrame', 'onDrawFrame', 'onFinishFrame', 'onResize')
        for i in xrange(100):
            name = pygame.event.event_name(i)
            if name != "Unknown":
                Event.attach(self, 'on' + name)

        self.renderSequence = [
            self.onSetupFrame,
            self.onDrawFrame,
            self.onFinishFrame,
            ]

        pygame.init()
        self.display = pygame.display
        self.resize(size)
        self.init()
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
        self.onResize()

    def setCaption(self, title):
        import pygame
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
        import pygame
        for event in pygame.event.get():
            self.onEvent(event)
        for f in self.renderSequence:
            f()

    def onFinishFrame(self):
        """Default onFinishFrame handler that flips the pygame buffers"""
        self.display.flip()

    def onEvent(self, event):
        import pygame
        getattr(self, 'on' + pygame.event.event_name(event.type))(event)

    def onQuit(self, event):
        self.eventLoop.stop()

    def onVideoResize(self, event):
        self.resize(event.size)

    def region(self, rect):
        """Return a class that represents a rectangular subsection of this viewport.
           This performs a shallow copy on ourselves, then subsurfaces its screen.
           """
        sub = copy.copy(self)
        sub.screen = self.secreen.subsurface(rect)
        sub.size = rect[2:]
        return sub


class OpenGLViewport(PygameViewport):
    """Subclasses PygameView to provide OpenGL initialization"""
    def init(self):
        from OpenGL import GL

        self.viewOrigin = (0,0)
        self.nearClip   = 3.0
        self.farClip    = 2500.0
        self.fov        = 45.0

        # Set up some common OpenGL defaults
        GL.glClearColor(0.0, 0.0, 0.0, 0.0)
        GL.glClearDepth(1.0)
        GL.glDepthFunc(GL.GL_LESS)
        GL.glShadeModel(GL.GL_SMOOTH)
        GL.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA)
        GL.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST)
        def onSetupFrame():
            GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)
        self.onSetupFrame.observe(onSetupFrame)

        self.onSetupFrame.observe(self.configureOpenGL)

    def configureOpenGL(self):
        from OpenGL import GL, GLU
        GL.glViewport(*(self.viewOrigin + self.size))
        GL.glMatrixMode(GL.GL_PROJECTION)
        GL.glLoadIdentity()
        GLU.gluPerspective(self.fov, float(self.size[0]) / self.size[1], self.nearClip, self.farClip)
        GL.glMatrixMode(GL.GL_MODELVIEW)
        GL.glLoadIdentity()

    def getModeFlags(self):
        import pygame
        return PygameViewport.getModeFlags(self) | pygame.OPENGL

    def setViewport(self, rect):
        self.viewOrigin = rect[:2]
        self.size = rect[2:]

    def region(self, rect):
        """Return a class that represents a rectangular subsection of this viewport.
           To maintain something resembling OpenGL state integrity, it disconnects
           the region's rendering events from ours and appends them to our rendering
           sequence.
           """
        sub = copy.copy(self)
        sub.setViewport(rect)
        sub.onSetupFrame  = Event.Event(sub.configureOpenGL)
        sub.onDrawFrame   = Event.Event()
        sub.onFinishFrame = Event.Event()

        # Stick it in our render sequence right before our onFinishFrame which flips the buffer
        self.renderSequence = self.renderSequence[:-1] + \
                              [sub.onSetupFrame, sub.onDrawFrame, sub.onFinishFrame] + \
                              self.renderSequence[-1:]
        return sub

### The End ###

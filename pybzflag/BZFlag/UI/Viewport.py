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
        Event.attach(self, 'onFrame', 'onSetupFrame', 'onDrawFrame', 'onFinishFrame', 'onResize')

        self.renderSequence = [
            self.onSetupFrame,
            self.onDrawFrame,
            self.onFinishFrame,
            ]
        self.visible = True

        # For subviews created with region()
        self.parent = None
        self.rootView = self

    def render(self):
        if self.visible and self.size[0] > 0 and self.size[1] > 0:
            for f in self.renderSequence:
                f()

    def setCaption(self, title):
        """Set the window caption on the viewport, if applicable"""
        pass

    def region(self, rect):
        """Return a new Viewport that identifies the given rectangular subviewport.
           rect is allowed to either be a list specifying a rectangle, or a callable
           object that returns such a list. If possible the callable should be lazily
           evaluated, but that behaviour depends on the subclass.
           """
        pass


class PygameViewport(Viewport):
    """A basic viewport for 2D pygame rendering. This handles all pygame
       events, and is subclassed below for 3D rendering via pygame.
       """
    def __init__(self, eventLoop, size=(640,480), targetFrameRate=60):
        import pygame
        Viewport.__init__(self, eventLoop)

        # Add event handlers for all the pygame events we can find
        for i in xrange(100):
            name = pygame.event.event_name(i)
            if name != "Unknown":
                Event.attach(self, 'on' + name)

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
        self.render()

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
        if callable(rect):
            rect = rect

        sub = copy.copy(self)
        sub.screen = self.secreen.subsurface(rect)
        sub.size = rect[2:]
        sub.parent = self

        # Ignore the caption on sub-viewports
        sub.setCaption = lambda title: None
        return sub


class OpenGLViewport(PygameViewport):
    """Subclasses PygameView to provide OpenGL initialization"""
    def init(self):
        from OpenGL import GL

        self.nearClip    = 0.1
        self.farClip     = 2500.0
        self.fov         = 45.0
        self.rectExp = [0,0] + list(self.size) # A function or list specifying our relative viewport
        self.rect    = self.rectExp            # Our absolute viewport
        self.wireframe   = False

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

        def onResize():
            self.rectExp = [0,0] + list(self.size)
        self.onResize.observe(onResize)

        self.onSetupFrame.observe(self.configureOpenGL)

    def evalViewport(self):
        """Evaluate our viewport if necessary, and set up our 'size' and 'viewport' members"""
        if callable(self.rectExp):
            v = self.rectExp()
        else:
            v = self.rectExp
        if self.parent:
            v = (v[0] + self.parent.rect[0],
                 v[1] + self.parent.rect[1],
                 v[2], v[3])
        self.rect = v
        self.size = v[2:]

    def configureOpenGL(self):
        from OpenGL import GL, GLU
        GL.glViewport(*self.rect)

        # Set up the projection matrix with the current viewport size and FOV.
        # If we have no FOV, set up an orthogonal mode scaled in pixels.
        GL.glMatrixMode(GL.GL_PROJECTION)
        GL.glLoadIdentity()
        self.setProjectionMatrix()
        GL.glMatrixMode(GL.GL_MODELVIEW)
        GL.glLoadIdentity()
        GL.glDepthRange(0.01,2000)

        # Enable/disable wireframe on a per-viewport basis
        if self.wireframe:
            GL.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_LINE);
        else:
            GL.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_FILL);

    def setProjectionMatrix(self):
        from OpenGL import GL, GLU
        if self.fov:
            GLU.gluPerspective(self.fov, float(self.size[0]) / self.size[1], self.nearClip, self.farClip)
        else:
            GL.glOrtho(0,self.size[0],0,self.size[1], -self.farClip, self.farClip)

    def getModeFlags(self):
        import pygame
        return PygameViewport.getModeFlags(self) | pygame.OPENGL

    def render(self):
        """Reevaluate our viewport, then do the usual rendering sequence"""
        self.evalViewport()
        PygameViewport.render(self)

    def region(self, rect):
        """Return a class that represents a rectangular subsection of this viewport.
           To maintain something resembling OpenGL state integrity, it disconnects
           the region's rendering events from ours and appends them to our rendering
           sequence.

           In addition to a rectangle, this function can accept a function that
           will be lazily evaluated to a rectangle each frame. This makes it possible
           to create regions with animated or dynamically sized positions.
           """
        sub = copy.copy(self)
        sub.parent = self
        sub.rectExp = rect
        sub.onSetupFrame  = Event.Event(sub.configureOpenGL)
        sub.onDrawFrame   = Event.Event()
        sub.onFinishFrame = Event.Event()
        sub.renderSequence = [sub.onSetupFrame,
                              sub.onDrawFrame,
                              sub.onFinishFrame]

        # Stick it in our render sequence right before our onFinishFrame which flips the buffer
        # This should be safe for nesting viewport regions-  and the last entry will always be
        # the root viewport's onFinishFrame event.
        self.rootView.renderSequence = self.rootView.renderSequence[:-1] + \
                                       [sub.render] + \
                                       self.rootView.renderSequence[-1:]

        # Ignore the caption on sub-viewports
        sub.setCaption = lambda title: None        
        return sub

### The End ###

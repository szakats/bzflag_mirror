""" BZFlag.UI.Viewport.OpenGL

A Viewport implementation for OpenGL. This uses Pygame to set up an OpenGL
context and receive events, hence this is a subclass of the Pygame viewport.
On top of PygameViewport, this class manages viewport-wide OpenGL context
details like field of view, clearing the color and Z buffers, and wireframe mode.

When should a feature go in the Viewport rather than in something like ThreeDRender?
If the feature should affect sub-viewports created by region(), or it's important
that it only ever be invoked once per frame, it might be a good cantidate to go here.
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

from Pygame import PygameViewport
from OpenGL.GL import *
from OpenGL.GLU import *
from BZFlag.UI import GLExtension
from BZFlag import Event
import pygame, copy


class ViewportMode:
    """Abstract base class for a mode that affects the rendering
       initialization and completion of an entire viewport.
       """
    def setupFrame(self):
        """Called during the viewport's onSetupFrame event"""
        pass

    def finishFrame(self):
        """Called during the viewport's onFinishFrame event, before the page flip"""
        pass


class ClearedMode(ViewportMode):
    """A viewport mode in which the color buffer is cleared each frame"""
    def __init__(self, clearColor=(0,0,0,1)):
        glClearColor(*clearColor)

    def setupFrame(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        


class OpenGLViewport(PygameViewport):
    """Subclasses PygameView to provide OpenGL initialization"""
    def init(self):
        # Test for OpenGL extensions we can use
        GLExtension.test()

        self.nearClip    = 0.1
        self.farClip     = 2500.0
        self.fov         = 45.0
        self.rectExp     = [0,0] + list(self.size)  # A function or list specifying our relative viewport
        self.rect        = self.rectExp             # Our absolute viewport
        self.clearColor  = (0,0,0,1)

        self.wireframe   = False
        self.wireframeClearColor = (0.5, 0.5, 0.5, 1)

        # Set up some common OpenGL defaults
        glClearDepth(1.0)
        glDepthFunc(GL_LESS)
        glShadeModel(GL_SMOOTH)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

        def onSetupFrame():
            # Always clear in wireframe mode, but in normal mode it can be optional.
            # Set clearColor to None to disable it.
            if self.wireframe or self.clearColor:
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
            else:
                glClear(GL_DEPTH_BUFFER_BIT)
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
        glViewport(*self.rect)

        # Set up the projection matrix with the current viewport size and FOV.
        # If we have no FOV, set up an orthogonal mode scaled in pixels.
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        self.setProjectionMatrix()
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glDepthRange(0.01,2000)

        # Enable/disable wireframe on a per-viewport basis
        if self.wireframe:
            glClearColor(*self.wireframeClearColor)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else:
            if self.clearColor:
                glClearColor(*self.clearColor)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    def setProjectionMatrix(self):
        if self.fov:
            gluPerspective(self.fov, float(self.size[0]) / self.size[1], self.nearClip, self.farClip)
        else:
            glOrtho(0,self.size[0],self.size[1],0, -self.farClip, self.farClip)

    def getModeFlags(self):
        return PygameViewport.getModeFlags(self) | pygame.OPENGL

    def render(self):
        """Reevaluate our viewport, then do the usual rendering sequence"""
        self.evalViewport()
        PygameViewport.render(self)

    def region(self, rect, renderLink='after'):
        """Return a class that represents a rectangular subsection of this viewport.
           To maintain something resembling OpenGL state integrity, it disconnects
           the region's rendering events from ours and appends them to our rendering
           sequence.

           In addition to a rectangle, this function can accept a function that
           will be lazily evaluated to a rectangle each frame. This makes it possible
           to create regions with animated or dynamically sized positions.

           renderLink controls how this region is linked into its parent's rendering
           sequence. 'after' inserts it after all other entries, 'before' before all
           others, None doesn't insert it.
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

        if renderLink == 'after':
            # Stick it in our render sequence right before our onFinishFrame which flips the buffer
            # This should be safe for nesting viewport regions-  and the last entry will always be
            # the root viewport's onFinishFrame event.
            self.rootView.renderSequence = self.rootView.renderSequence[:-1] + \
                                           [sub.render] + \
                                           self.rootView.renderSequence[-1:]

        if renderLink == 'before':
            self.rootView.renderSequence = [sub.render] + self.rootView.renderSequence

        # Ignore the caption on sub-viewports
        sub.setCaption = lambda title: None
        return sub

class StereoGLViewport(OpenGLViewport):
    def getModeFlags(self):
        # use this as a convenient way to set the GL options
	pygame.display.gl_set_attribute(pygame.GL_STEREO, 1)
	return OpenGLViewport.getModeFlags(self)

### The End ###

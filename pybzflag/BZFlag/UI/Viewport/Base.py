""" BZFlag.UI.Viewport.Base

An abstract base class defining the Viewport interface
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

from BZFlag import Event
import copy

__all__ = ('Viewport',)


class Viewport:
    """Abstract base class for a viewport. Defines events and methods
       that all viewports must support.
       """
    def __init__(self, eventLoop, size):
        self.eventLoop = eventLoop
        Event.attach(self, 'onFrame', 'onSetupFrame', 'onDrawFrame', 'onFinishFrame', 'onResize')
        self.renderSequence = [
            self.onSetupFrame,
            self.onDrawFrame,
            self.onFinishFrame,
            ]
        self.visible = True

        self.parent = None
        self.rootView = self
        self.children = []

        self.rect = None
        self.rectExp = None
        self.size = None
        self.setRect((0,0) + size)

    def render(self):
        if self.visible and self.size[0] > 0 and self.size[1] > 0:
            for f in self.renderSequence:
                f()

    def setRect(self, rect):
        """Set this viewport's size and position rectangle.
           The provided rectangle can optionally be a function called to
           calculate the viewport's rectangle any time updateRect is called.
           """
        if callable(rect):
            self.rectExp = rect
        else:
            self.rectExp = lambda: rect
        self.updateRect()

    def updateRect(self):
        """Actually resize the viewport, calculating a rectangle using the
           expression stored by setRect().
           """
        newRect = self.rectExp()
        if newRect != self.rect:
            self.rect = newRect
            if self.parent:
                self.rect = (self.rect[0] + self.parent.rect[0],
                             self.rect[1] + self.parent.rect[1],
                             self.rect[2],
                             self.rect[3])
            self.size = self.rect[2:]
            self.onResize()
            for child in self.children:
                child.updateRect()

    def setCaption(self, title):
        """Set the window caption on the viewport, if applicable"""
        pass

    def region(self, rect, renderLink='after'):
        """Return a new Viewport that identifies the given rectangular subviewport.
           As in setRect, the rectangle can also be a function that returns a viewport,
           to be reevaluated when a parent viewport changes size.

           renderLink controls how this region is linked into its parent's rendering
           sequence. 'after' inserts it after all other entries, 'before' before all
           others, None doesn't insert it.
           """
        sub = copy.copy(self)
        sub.parent = self
        sub.children = []
        self.children.append(sub)
        sub.setRect(rect)

        # Disconnect events and the renderSequence from the parent
        sub.onFrame       = Event.Event()
        sub.onSetupFrame  = Event.Event()
        sub.onDrawFrame   = Event.Event()
        sub.onFinishFrame = Event.Event()
        sub.onResize      = Event.Event()
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

### The End ###

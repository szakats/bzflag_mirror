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
import copy, weakref

__all__ = ('Viewport', 'RegionMixin')


class Viewport(object):
    """Abstract base class for a viewport. Defines events and methods
       that all viewports must support.
       """
    def __init__(self, eventLoop, size=None):
        self.eventLoop = eventLoop
        Event.attach(self, 'onFrame', 'onSetupFrame', 'onDrawFrame', 'onFinishFrame', 'onResize')

        # This makes it easier for RegionMixin to override the parent and rootView
        # while still having it set correctly for show() below.
        if not hasattr(self, 'parent'):
            self.parent = None
            self.rootView = self

        self.rect = None
        self.rectExp = None
        self.size = None
        if size is not None:
            self.setRect((0,0) + size)

        self.renderSequence = [
            self.onSetupFrame,
            self.onDrawFrame,
            self.onFinishFrame,
            ]

        self.visible = False
        self.show()

    def show(self):
        """Make this viewport visible. the 'visible' attribute must be treated as
           read-only. This implementation simply sets it, but viewport regions have
           a much more complex procedure.
           """
        self.visible = True

    def hide(self):
        self.visible = False

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
        class ViewportRegion(RegionMixin, self.__class__):
            pass
        return ViewportRegion(self, rect, renderLink)


class RegionMixin(Viewport):
    """A mix-in class that implements sub-viewports created with the region() method.
       This class may perform extra implementation-specific per-region setup.

       parent     - The viewport who had its region() called to create this region
       rect       - A 4-tuple specifying the x, y, width, and height coordinates of this region,
                    relative to the parent.
       renderLink - Specifies how this viewport is inserted into its parent's rendering sequence.
                    'after' inserts it at the end of the rendering sequence, 'before' inserts it
                    at the beginning, None doesn't insert it at all.
       """
    def __init__(self, parent, rect, renderLink):
        self.renderLink = renderLink
        self.parent = parent
        self.rootView = parent.rootView

        # Create a weak reference that proxies our render() function.
        # This is used in setting up the parent's renderSequence in show(),
        # so that it doesn't hold any strong references to us.
        self.renderProxy = RenderSequenceProxy(self.rootView, self)

        # Note that we call Viewport's init, rather than our direct subclass.
        # We don't want to initialize the viewport's underlying video device again.
        Viewport.__init__(self, parent.eventLoop)
        self.setRect(rect)

        # Inherit any remaining attributes from the parent
        for attribute, value in parent.__dict__.iteritems():
            if not hasattr(self, attribute):
                setattr(self, attribute, value)

    def setCaption(self, title):
        """Viewport regions have nowhere to show a caption"""
        pass

    def show(self):
        """For viewport regions, this attaches the viewport into the parent's render cycle."""
        if self.visible:
            return
        self.visible = True

        # Update our rectangle when the parent's changes
        self.parent.onResize.observe(self.updateRect)

        if self.renderLink == 'after':
            # Stick it in our render sequence right before our onFinishFrame which flips the buffer
            # This should be safe for nesting viewport regions-  and the last entry will always be
            # the root viewport's onFinishFrame event.
            self.rootView.renderSequence.insert(len(self.rootView.renderSequence) - 1,
                                                self.renderProxy)

        elif self.renderLink == 'before':
            self.rootView.renderSequence.insert(0, self.renderProxy)

    def hide(self):
        """Make the viewport invisible. Invisible viewport regions don't take any time
           during the render cycle, whereas visible viewports always have some overhead
           even if they have no handlers attached.
           """
        if not self.visible:
            return
        self.visible = False

        # Stop updating our parent rectangle
        parent.onResize.unobserve(self.updateRect)

        # Try to remove our renderProxy from the renderSequence. If this fails, it could
        # just mean that renderProxy was None when we were last show()'ed, so ignore it.
        try:
            self.rootView.renderSequence.remove(self.renderProxy)
        except ValueError:
            pass


class RenderSequenceProxy:
    """This is a class that is inserted into the Viewport's renderSequence as
       a weak reference proxy. When the referenced object is garbage collected,
       the viewport's renderSequence entry corresponding to this proxy is deleted.
       """
    def __init__(self, rootViewport, subViewport):
        self.root = rootViewport
        self.subRef = weakref.ref(subViewport, self.unref)

    def __call__(self, *args, **kw):
        self.subRef().render(*args, **kw)

    def unref(self, ref):
        try:
            self.root.renderSequence.remove(self)
        except ValueError:
            # This won't be in the list if our viewport isn't visible
            pass

### The End ###

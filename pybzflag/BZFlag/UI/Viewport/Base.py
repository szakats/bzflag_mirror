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

__all__ = ('Viewport',)


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

### The End ###

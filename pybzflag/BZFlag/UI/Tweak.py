""" BZFlag.UI.Tweak

A system for building a UI that runs in a separate thread, adjusting
parameters in a real-time rendering or simulation.

This implementation uses pygtk.
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

from __future__ import division
import gtk, threading


class Thread(threading.Thread):
    """The thread running GTK's main loop, and therefore all the widget callbacks"""
    def __init__(self):
        threading.Thread.__init__(self)
        self.start()

    def run(self):
        """The thread's run function, started by start()"""
        gtk.threads_init()
        gtk.main()

    def stop(self):
        """Force the gtk event loop to terminate"""
        gtk.main_quit()


class Window:
    """A window that holds a list of named tweaking controls"""
    def __init__(self, *controls):
        self.controls = controls

        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_border_width(8)
        self.window.set_default_size(600,0)

        self.table = gtk.Table(len(controls), 2)
        self.window.add(self.table)
        self.table.show()

        row = 0
        for control in controls:
            l = gtk.Label(control.name + ": ")
            l.set_alignment(1, 0.75)
            self.table.attach(l, 0,1, row,row+1, xoptions=gtk.FILL, yoptions=gtk.FILL)
            l.show()

            self.table.attach(control.widget, 1,2, row,row+1, yoptions=gtk.FILL)
            control.widget.show()
            row += 1

        self.window.show()


class Control:
    """A control which can be placed in a Tweak.Window"""
    def __init__(self, name, widget):
        self.name = name
        self.widget = widget


class Attribute(Control):
    """A control that lets you directly manipulate a scalar or vector object attribute"""
    def __init__(self, object, attribute, range=(0,1), name=None, stepSize=None, pageSize=None):
        self.object = object
        self.attribute = attribute

        if not name:
            name = "%s.%s" % (object.__class__.__name__, attribute)
        if not stepSize:
            stepSize = (range[1] - range[0]) / 200
        if not pageSize:
            pageSize = (range[1] - range[0]) / 50
        initialValue = getattr(object, attribute)

        # Create a list of gtk Adjustments for our attribute
        adjustments = []
        try:
            # First assume it's a vector attribute
            for index in xrange(len(initialValue)):
                adj = gtk.Adjustment(initialValue[index], range[0], range[1], stepSize, pageSize)
                adj.index = index
                adj.connect("value_changed", lambda adj: self.updateIndex(adj))
                adjustments.append(adj)
        except TypeError:
            # Nope, now assume it's a scalar
            adj = gtk.Adjustment(initialValue, range[0], range[1], stepSize, pageSize)
            adj.connect("value_changed", self.update)
            adjustments.append(adj)

        # Build corresponding HScales inside a HBox
        box = gtk.HBox(gtk.FALSE, gtk.FALSE)
        for adj in adjustments:
            scale = gtk.HScale(adj)
            scale.set_digits(3)
            box.pack_start(scale, gtk.TRUE, gtk.TRUE, 0)
            scale.show()

        Control.__init__(self, name, box)

    def update(self, adj):
        setattr(self.object, self.attribute, adj.value)

    def updateIndex(self, adj):
        index = adj.index
        try:
            getattr(self.object, self.attribute)[index] = adj.value
        except TypeError:
            # If it's a tuple, it won't support setting indices directly, we have to build a new object
            l = list(getattr(self.object, self.attribute))
            l[index] = adj.value
            setattr(self.object, self.attribute, l)


def run(runnable):
    """A utility function to create a Tweak.Thread, call runnable.run(), then
       safely tell the Tweak.Thread to stop. This is designed to be used with
       an event loop, but should suppport any runnable object.
       """
    try:
        tweakThread = Thread()
        runnable.run()
    finally:
        tweakThread.stop()

### The End ###

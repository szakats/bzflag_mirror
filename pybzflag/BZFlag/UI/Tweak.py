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


class AttributeControl(Control):
    """A control which operates on a vector or scalar object attribute"""
    def __init__(self, object, attribute, name=None):
        self.object = object
        self.attribute = attribute
        if not name:
            name = "%s.%s" % (object.__class__.__name__, attribute)

        # Get the subclass to add subwidgets for each individual value
        box = gtk.HBox(gtk.FALSE, gtk.FALSE)
        initialValue = getattr(object, attribute)
        try:
            # First assume it's a vector attribute
            for index in xrange(len(initialValue)):
                def createUpdater(index):
                    # This is necessary so 'index' in the lambda binds with the value at this
                    # iteration, rather than the final value after the loop exits.
                    return lambda value: self.updateIndex(value, index)
                w = self.add(initialValue[index],  createUpdater(index))
                box.pack_start(w, gtk.TRUE, gtk.TRUE, 0)
                w.show()
        except TypeError:
            # Nope, now assume it's a scalar
            w = self.add(initialValue, self.update)
            box.pack_start(w, gtk.TRUE, gtk.TRUE, 0)
            w.show()

        Control.__init__(self, name, box)

    def add(self, initialValue, setFunction):
        """A hook for subclasses to handle adding individual values
           in the attribute. initialValue is the value at initialization,
           setFunction should be called to change it. This should return
           a widget, which will be packed into the control's HBox.
           """
        pass

    def update(self, value):
        setattr(self.object, self.attribute)

    def updateIndex(self, value, index):
        try:
            getattr(self.object, self.attribute)[index] = value
        except TypeError:
            # If it's a tuple, it won't support setting indices directly, we have to build a new object
            l = list(getattr(self.object, self.attribute))
            l[index] = value
            setattr(self.object, self.attribute, l)


class Quantity(AttributeControl):
    """A control that lets you directly manipulate a scalar or
       vector object attribute, using the gtk Scale widget.
       """
    def __init__(self, object, attribute, range=(0,1), name=None, stepSize=None, pageSize=None):
        self.range = range
        if not stepSize:
            stepSize = (range[1] - range[0]) / 200
        if not pageSize:
            pageSize = (range[1] - range[0]) / 50
        self.stepSize = stepSize
        self.pageSize = pageSize
        AttributeControl.__init__(self, object, attribute, name)

    def add(self, initialValue, setFunction):
        print (initialValue, self.range[0], self.range[1], self.stepSize, self.pageSize)
        adj = gtk.Adjustment(initialValue, self.range[0], self.range[1], self.stepSize, self.pageSize)
        adj.connect("value_changed", lambda adj: setFunction(adj.value))
        scale = gtk.HScale(adj)
        scale.set_digits(3)
        return scale


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

#!/usr/bin/env python
#
# Simple viewer for the 3D models PyBZFlag uses, in a subset of the VRML format.
# Give the file name or URL to a .wrl model on the command line.
#
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl
from BZFlag.UI.Drawable import Box
from BZFlag.Event import EventLoop
import sys

try:
    fileName = sys.argv[1]
except IndexError:
    print "A model filename or URI must be specified on the command line."
    sys.exit(0)

# Set up a quick 3D renderer view with a controller attached so we can spin the model around
loop = EventLoop()
viewport = Viewport.OpenGLViewport(loop, (800,600))
viewport.setCaption("Model Viewer - %s" % fileName)
view = ThreeDRender.View(viewport)
control = ThreeDControl.Viewing(view, viewport)

class ViewerModel:
    pass

view.scene.objects[ViewerModel()] = [Box.BoxSides( (0,0,0), 0, (100,100,50) ),
                                     Box.BoxTops( (0,0,0), 0, (100,100,50) )]
view.scene.preprocess()
loop.run()



#!/usr/bin/env python
#
# Simple viewer for the 3D models PyBZFlag uses, in a subset of the VRML format.
# Give the file name or URL to a .wrl model on the command line.
#
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl
from BZFlag.UI.Drawable import Box, VRML
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

# Move the camera a little closer in, and down to the origin
view.camera.position = (0,0,0)
view.camera.distance = 60
view.camera.jump()

# Read in the VRML file, putting each mesh in a separate object
r = VRML.Reader(fileName)
class MeshObject:
    def __init__(self, name):
        self.name = name
    def __repr__(self):
        return "<%s %r>" % (self.__class__.__name__, self.name)
for name, drawable in r.meshes.items():
    view.scene.add(MeshObject(name), [drawable])

# Let the user pick objects with the left mouse button, printing the mesh name
def onMouseButtonDown(event):
    if event.button == 1:
        print 'picked: %r' % view.pick(event.pos)
viewport.onMouseButtonDown.observe(onMouseButtonDown)

view.scene.preprocess()
loop.run()

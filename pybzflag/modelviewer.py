#!/usr/bin/env python
#
# Simple viewer for the 3D models PyBZFlag uses, in a subset of the VRML format.
# Give the file name or URL to a .wrl model on the command line.
#
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, AnaglyphView, StereoView
from BZFlag.UI.Drawable import Box, VRML
from BZFlag import Event, optik
import sys, os


# Command line processing
parser = optik.OptionParser(usage = "usage: %prog [options] model.wrl")
parser.add_option("-a", "--anaglyph", action="store_true",
                  help="Enables anaglyph stereo mode.")
parser.add_option("-s", "--stereo", action="store_true",
                  help="Enables dual-channel stereo mode.")
(options, args) = parser.parse_args()

try:
    fileName = args[0]
except IndexError:
    print "A model filename must be specified on the command line."
    sys.exit(0)

if options.anaglyph:
    viewClass = AnaglyphView.AnaglyphView
elif options.stereo:
    viewClass = StereoView.StereoView    
else:
    viewClass = ThreeDRender.View
    

# Set up a quick 3D renderer view with a controller attached so we can spin the model around
loop = Event.EventLoop()
viewport = Viewport.OpenGLViewport(loop, (800,600))
viewport.setCaption("Model Viewer - %s" % fileName)
view = viewClass(viewport)
control = ThreeDControl.Viewing(view, viewport)


# Move the camera a little closer in, and down to the origin
view.camera.position = (0,0,0)
view.camera.distance = 60
view.camera.jump()


# Read in the VRML file, putting each mesh in a separate object
meshes = VRML.load(os.path.abspath(fileName))
class MeshObject:
    def __init__(self, name):
        self.name = name
    def __repr__(self):
        return "<%s %r>" % (self.__class__.__name__, self.name)
for name, drawable in meshes.items():
    view.scene.add(MeshObject(name), [drawable])


# Let the user pick objects with the left mouse button, printing the mesh name
def onMouseButtonDown(event):
    if event.button == 1:
        print 'picked: %r' % view.pick(event.pos)
viewport.onMouseButtonDown.observe(onMouseButtonDown)


view.scene.preprocess()
loop.run()

#!/usr/bin/env python
#
# Test rig for our GLText module
#
from BZFlag.UI import Viewport, GLText
from BZFlag.Event import EventLoop
from BZFlag import Animated, Util
from OpenGL.GL import *

# Create a new orthogonal mode viewport
loop = EventLoop()
viewport = Viewport.OpenGLViewport(loop, (640,480))
viewport.setCaption("Font Test")
viewport.fov = None   

f = GLText.Font("Vera.ttf")

time = Animated.Timekeeper()
spin = Animated.Value(0, Animated.Velocity(-180))

def drawFrame():
    glLoadIdentity()
    glTranslatef(0,viewport.size[1],0)

    glColor3f(1,1,1)
    f.draw("Boing! This is the default size.\n")
    glColor3f(1,1,0.5)
    f.draw("This is size 50", 50)

    glLoadIdentity()
    glTranslatef(viewport.size[0]/2, viewport.size[1]/2, 0)
    glRotatef(spin.value, 0,0,1)
    spin.integrate(time.step())

    f.drawCentered("Spinny-widget")

    glLoadIdentity()
    glTranslatef(viewport.size[0]/2, viewport.size[1]/4, 0)

    f.drawCentered("Not-so-spinny-widget",15)

Util.showFrameRate(viewport)
viewport.onDrawFrame.observe(drawFrame)
loop.run()



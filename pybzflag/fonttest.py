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
viewport = Viewport.OpenGLViewport(loop, (640,480), 1000)
viewport.setCaption("Font Test")
viewport.fov = None

time = Animated.Timekeeper()
spin = Animated.Value(Animated.Velocity(80))
zoom = Animated.Value(Animated.SineFunction(0.5, (0.8,1)))

def drawFrame():
    # Gradient
    glLoadIdentity()
    glDisable(GL_TEXTURE_2D)
    glBegin(GL_QUADS)
    glColor3f(0,0,0)
    glVertex2f(0,0)
    glVertex2f(viewport.size[0],0)
    glColor3f(1,1,1)
    glVertex2f(viewport.size[0],viewport.size[1])
    glVertex2f(0,viewport.size[1])
    glEnd()

    glLoadIdentity()
    glTranslatef(5,5,0)
    glColor3f(1,1,1)
    GLText.draw("Boing! This is the default size\n\n")
    glColor3f(1,1,0.5)
    GLText.draw("50 pixels high", 50)

    glLoadIdentity()
    glTranslatef(viewport.size[0]/2, viewport.size[1]/2, 0)
    glRotatef(spin.value, 0,0,1)
    glScalef(zoom.value, zoom.value, zoom.value)
    dt = time.step()
    spin.integrate(dt)
    zoom.integrate(dt)
    GLText.drawCentered("Spinny-widget", 30)

    glColor3f(0,0,0)
    glLoadIdentity()
    glTranslatef(viewport.size[0]/2, viewport.size[1] * 0.75, 0)
    GLText.drawCentered("Not-so-spinny-widget")

    glLoadIdentity()
    glTranslatef(viewport.size[0]/2, viewport.size[1] * 0.85, 0)
    GLText.drawCentered("(small print)", 10)

    glLoadIdentity()
    glTranslatef(viewport.size[0]/2, viewport.size[1] * 0.95, 0)
    glColor3f(0,0,1)
    GLText.drawCentered("bold", fontName="VeraBd.ttf")

Util.showFrameRate(viewport)
viewport.onDrawFrame.observe(drawFrame)
loop.run()



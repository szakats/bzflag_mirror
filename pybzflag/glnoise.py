#!/usr/bin/env python
from BZFlag.UI import Viewport, Texture
from BZFlag.Event import EventLoop
from BZFlag import Animated, Util, Noise
from OpenGL.GL import *
from Numeric import *

class NoiseTexture(Texture.Texture):
    def __init__(self, size=(256,256), seed=None):
        Texture.Texture.__init__(self)
        r = Noise.randomArray(size, seed, (0,255), UInt8)
        self.loadRaw(size, r.tostring(), GL_LUMINANCE, 1)
        self.setFilter(GL_NEAREST, GL_NEAREST)
        

if __name__ == '__main__':
    loop = EventLoop()
    viewport = Viewport.OpenGLViewport(loop, (640,480), 1000)
    viewport.setCaption("OpenGL Noise Experiment")
    viewport.fov = None   

    repeats = 30
    rotationDistance = 100
    rotationSpeed = 200

    time = Animated.Timekeeper()
    spin = Animated.Value(Animated.Velocity(rotationSpeed))
    noise = NoiseTexture()

    def drawFrame():
        dt = time.step()
        spin.integrate(dt)

        glEnable(GL_TEXTURE_2D)
        noise.bind()

        glMatrixMode(GL_TEXTURE)
        glLoadIdentity()
        glRotatef(spin.value, 0,0,1)
        glTranslatef(-rotationDistance, -rotationDistance, 0)
        glMatrixMode(GL_MODELVIEW)
        
        glBegin(GL_QUADS)
        glTexCoord2f(0,0)
        glVertex2f(0,0)
        glTexCoord2f(repeats,0)
        glVertex2f(viewport.size[0],0)
        glTexCoord2f(repeats,repeats)
        glVertex2f(*viewport.size)
        glTexCoord2f(0,repeats)
        glVertex2f(0,viewport.size[1])
        glEnd()

    Util.showFrameRate(viewport)
    viewport.onDrawFrame.observe(drawFrame)
    loop.run()

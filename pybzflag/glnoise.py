#!/usr/bin/env python
from __future__ import division
from BZFlag.Event import EventLoop
from BZFlag.UI import Viewport, GLNoise, Texture
from BZFlag import Util, Animated
from OpenGL.GL import *

loop = EventLoop()
viewport = Viewport.OpenGLViewport(loop, (800,600), 1000)
viewport.setCaption("OpenGL Noise Experiment")
viewport.fov = None   

time = Animated.Timekeeper()
r = GLNoise.NoiseRenderer()    
octaves = None

persistence = 0.75
period = 50

def drawFrame():
    global octaves, r, time, persistence
    if not octaves:
        octaves = (
                   GLNoise.AnimatedNoise(r, (256,256), 32, period/128),
                   GLNoise.AnimatedNoise(r, (128,128), 32, period/64),
                   GLNoise.AnimatedNoise(r, (64,64),   32, period/32),
                   GLNoise.AnimatedNoise(r, (32,32),   32, period/16),
                   GLNoise.AnimatedNoise(r, (16,16),   32, period/8),
                   GLNoise.AnimatedNoise(r, (8,8),     32, period/4),
                   GLNoise.AnimatedNoise(r, (4,4),     32, period/2),
                   GLNoise.AnimatedNoise(r, (2,2),     32, period),
                   )

    dt = time.step()
    for octave in octaves:
        octave.integrate(dt)
        octave.draw(viewport.size, (1,1,1, 1-persistence))

## octaves = [Texture.Texture() for i in xrange(9)]
## persistence = 0.75

## def drawFrame():
##     r.reset()
##     r.render(octaves[8], (2,2))
##     r.render(octaves[7], (4,4))        
##     r.render(octaves[6], (8,8))        
##     r.render(octaves[5], (16,16))        
##     r.render(octaves[4], (32,32))        
##     r.render(octaves[3], (64,64))        
##     r.render(octaves[2], (128,128))        
##     r.render(octaves[1], (256,256))
##     r.render(octaves[0], (512,512))

##     glEnable(GL_TEXTURE_2D)
##     glColor3f(1,1,1)

##     for octave in octaves:
##         octave.bind()

##         glBegin(GL_QUADS)
##         glTexCoord2f(0,0)
##         glVertex2f(0,0)
##         glTexCoord2f(2,0)
##         glVertex2f(viewport.size[0],0)
##         glTexCoord2f(2,2)
##         glVertex2f(*viewport.size)
##         glTexCoord2f(0,2)
##         glVertex2f(0,viewport.size[1])
##         glEnd()

##         glEnable(GL_BLEND)
##         glColor4f(1,1,1,1-persistence)

## def keyDown(event):
##     global persistence, r, seed
##     if event.unicode == '[':
##         persistence -= 0.05
##     elif event.unicode == ']':
##         persistence += 0.05
##     print "Persistence: %s" % persistence
## viewport.onKeyDown.observe(keyDown)

Util.showFrameRate(viewport)
viewport.onDrawFrame.observe(drawFrame)
loop.run()

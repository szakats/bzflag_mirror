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
n = GLNoise.AnimatedPerlinNoise()

def drawFrame():
    n.integrate(time.step())
    n.draw(viewport.size)

Util.showFrameRate(viewport)
viewport.onDrawFrame.observe(drawFrame)
loop.run()

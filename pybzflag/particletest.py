#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable
from BZFlag import Event, Animated
from Numeric import *
from OpenGL.GL import *


class GlowSphere(Drawable.ParticleArray):
    """An example particle system that draws glowing balls positioned on the surface of a sphere"""
    def __init__(self, numParticles=100, particleDiameter=10, sphereDiameter=100):
        Drawable.ParticleArray.__init__(self, (numParticles,), particleDiameter)

if __name__ == '__main__':
    loop = Event.EventLoop()
    viewport = Viewport.OpenGLViewport(loop)
    view = ThreeDRender.View(viewport)
    control = ThreeDControl.Viewing(view, viewport)
    viewport.setCaption("Particle System Test")

    viewport.mode = Viewport.GL.ClearedMode(clearColor=(0.2, 0.2, 0.2, 1))

    view.camera.position = (0,0,0)
    view.camera.distance = 60
    view.camera.jump()

    view.scene.add(GlowSphere())

    loop.run()

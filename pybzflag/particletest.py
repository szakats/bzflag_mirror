#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable, ParticleSystem
from BZFlag import Event, Geometry, Noise, Animated
from OpenGL.GL import *
import math


class GlowSphere(Drawable.SpriteArray):
    """An example particle system that draws glowing balls positioned on the surface of a sphere"""
    textureName = 'spark.png'

    def __init__(self, numParticles=100):
        self.model = ParticleSystem.SpriteFountain(numParticles)
        Drawable.SpriteArray.__init__(self, numParticles)
        self.model.attachDrawable(self)

        self.time = Animated.Timekeeper()
        self.render.static = False
        self.render.blended = True

        self.model.add(ParticleSystem.Emitter, 10)
        self.model.add(ParticleSystem.LifespanAffector, 10)
        self.model.add(ParticleSystem.LinearFadeAffector, sizeRange=(0,100))

    def draw(self, rstate):
        self.model.integrate(self.time.step())
        glDisable(GL_LIGHTING)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE)
        Drawable.SpriteArray.draw(self, rstate)
        glEnable(GL_LIGHTING)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)


if __name__ == '__main__':
    loop = Event.EventLoop()
    viewport = Viewport.OpenGLViewport(loop)
    view = ThreeDRender.View(viewport)
    control = ThreeDControl.Viewing(view, viewport)
    viewport.setCaption("Particle System Test")

    viewport.mode = Viewport.GL.ClearedMode(clearColor=(0.2, 0.2, 0.2, 1))

    view.camera.position = (0,0,0)
    view.camera.distance = 150
    view.camera.jump()

    view.scene.add(GlowSphere())

    loop.run()

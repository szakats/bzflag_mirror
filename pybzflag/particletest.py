#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable, ParticleSystem
from BZFlag import Event, Geometry, Noise, Animated
from OpenGL.GL import *
import math


class GlowSphere(Drawable.ParticleArray):
    """An example particle system that draws glowing balls positioned on the surface of a sphere"""
    textureName = 'spark.png'

    def __init__(self, numParticles=2500, particleDiameter=5, gridRadius=80):
        resolution = int(math.sqrt(numParticles))
        self.model = ParticleSystem.Newtonian(Geometry.pointGrid(
            (-gridRadius, -gridRadius, 0), (gridRadius*2,0,0), (0,gridRadius*2,0), (resolution, resolution)))

        Drawable.ParticleArray.__init__(self, self.model.state.shape[:-1])
        self.model.attachDrawable(self)
        self.time = Animated.Timekeeper()

        self.sizes *= particleDiameter

        self.render.static = False
        self.render.blended = True

    def draw(self, rstate):
        self.model.integrate(self.time.step())
        glDisable(GL_LIGHTING)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE)
        Drawable.ParticleArray.draw(self, rstate)
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

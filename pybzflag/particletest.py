#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable
from BZFlag import Event, Animated, Geometry
from OpenGL.GL import *
import random


class GlowSphere(Drawable.ParticleArray):
    """An example particle system that draws glowing balls positioned on the surface of a sphere"""
    textureName = 'spark.png'

    def __init__(self, numParticles=500, particleDiameter=5, sphereDiameter=50):
        Drawable.ParticleArray.__init__(self, (numParticles,), particleDiameter)

        self.render.static = False
        self.render.blended = True

        # Generate some points on a sphere
        for i in xrange(numParticles):
            self.points[i] = [random.random() - 0.5 for j in xrange(3)]
        Geometry.normalize(self.points, self.points)
        self.points *= sphereDiameter

    def draw(self, rstate):
        glDisable(GL_LIGHTING)
        Drawable.ParticleArray.draw(self, rstate)
        glEnable(GL_LIGHTING)


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

#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable
from BZFlag import Event, Animated, Geometry
from OpenGL.GL import *
import random
import RandomArray


class GlowSphere(Drawable.ParticleArray):
    """An example particle system that draws glowing balls positioned on the surface of a sphere"""
    textureName = 'spark.png'

    def __init__(self, numParticles=2500, particleDiameter=5, sphereDiameter=50):
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
        glBlendFunc(GL_SRC_ALPHA, GL_ONE)
        Drawable.ParticleArray.draw(self, rstate)
        glEnable(GL_LIGHTING)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)


class WigglyGlowSphere(GlowSphere):
    def __init__(self):
        GlowSphere.__init__(self, numParticles=1000)
        self.time  = Animated.Timekeeper()
        self.wigglyness = 50

    def draw(self, rstate):
        dt = self.time.step()
        self.points += (RandomArray.random(self.points.shape)-0.5) * self.wigglyness * dt
        GlowSphere.draw(self, rstate)


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

    view.scene.add(WigglyGlowSphere())

    loop.run()

#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable
from BZFlag.UI import ParticleSystem, Environment, Tweak
from BZFlag import Event, Geometry, Noise, Animated
from OpenGL.GL import *
from Numeric import *


class Sparks(Drawable.SpriteArray):
    textureName = 'spark.png'
    def __init__(self, position=(0,0,0)):
        numParticles = 200
        self.model = ParticleSystem.SpriteFountain(numParticles)
        Drawable.SpriteArray.__init__(self, numParticles, allowPointSprite=False)
        self.model.attachDrawable(self)

        self.time = Animated.Timekeeper()
        self.render.static = False
        self.render.blended = True

        self.emitter = self.model.add(ParticleSystem.RandomEmitter,
                                      spawnRate           = 95,
                                      speedRange          = (4, 30),
                                      direction           = (0, 0, 1),
                                      directionRandomness = 0.2,
                                      position            = position,
                                      )
        self.model.add(ParticleSystem.LifespanAffector, 1)
        self.model.add(ParticleSystem.FountainFadeAffector,
                       sizeRange = (0, 1),
                       )
        self.model.add(ParticleSystem.ConstantAccelAffector, (0,0,-50))

    def draw(self, rstate):
        self.model.integrate(self.time.step())
        glDisable(GL_LIGHTING)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE)
        Drawable.SpriteArray.draw(self, rstate)
        glEnable(GL_LIGHTING)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)


class Smoke(Drawable.SpriteArray):
    textureName = 'cloud.png'
    def __init__(self, position=(0,0,0)):
        numParticles = 250
        self.model = ParticleSystem.SpriteFountain(numParticles)
        Drawable.SpriteArray.__init__(self, numParticles, allowPointSprite=False)
        self.model.attachDrawable(self)

        self.time = Animated.Timekeeper()
        self.render.static = False
        self.render.blended = True

        self.emitter = self.model.add(ParticleSystem.RandomEmitter,
                                      spawnRate           = 75,
                                      speedRange          = (0, 2),
                                      direction           = (0, 0, 1),
                                      directionRandomness = 1,
                                      position            = position,
                                      )
        self.model.add(ParticleSystem.LifespanAffector, 3)
        self.model.add(ParticleSystem.FountainFadeAffector,
                       sizeRange           = (6, 1),
                       colorRange          = ((0.5, 0.5, 0.5, 0  ),
                                              (1  , 1  , 1  , 0.1)),
                       colorFunction       = lambda x: 1-pow(1-x, 10),
                       )
        self.model.add(ParticleSystem.ConstantAccelAffector, (0,0,3))

    def draw(self, rstate):
        self.model.integrate(self.time.step())
        glDisable(GL_LIGHTING)
        Drawable.SpriteArray.draw(self, rstate)
        glEnable(GL_LIGHTING)


if __name__ == '__main__':
    loop = Event.EventLoop()
    viewport = Viewport.OpenGLViewport(loop)
    view = ThreeDRender.View(viewport)
    control = ThreeDControl.Viewing(view, viewport)
    viewport.setCaption("Particle System Test")

    viewport.mode = Viewport.GL.ClearedMode(clearColor=(0.2, 0.2, 0.2, 1))

    view.camera.position = (0,0,6)
    view.camera.distance = 20
    view.camera.jump()

    sky = Environment.Sky()
    view.scene.add(sky)
    viewport.onSetupFrame.observe(sky.update)
    view.scene.add(Drawable.Ground(400))

    #smoke = Smoke((-5,0,3))
    #view.scene.add(smoke)

    sparks = Sparks((0,0,3))
    view.scene.add(sparks)

    Tweak.Window(Tweak.Scalar(view.camera, 'distance', maximum=100),
                 Tweak.Scalar(sparks.emitter, 'spawnRate', maximum=500),
                 )

    Tweak.run(loop)

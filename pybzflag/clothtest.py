#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable, SpringSystem, Texture
from BZFlag import Event, Util
from Numeric import *
from time import time
from OpenGL.GL import *


class AnchorAffector(SpringSystem.Affector):
    def integrate(self, dt):
        # Pin the cloth up at its two left corners
        self.model.state[-1,0] = self.model.initialState[-1,0]
        self.model.state[0,0] = self.model.initialState[0,0]


class FlagColorizer(Drawable.Transform):
    def apply(self):
        glColor3f(0.5,1,0.5)


class Flag:
    def __init__(self):
        self.pole = Drawable.VRML.load("flagpole.wrl")
        self.cloth = SpringSystem.Cloth(self.getInitialState())

        surf = Drawable.ArraySurface(self.cloth.state, self.getTexCoords())
        surf.render.static = False
        surf.render.texture = Texture.load("flag.png")
        self.transformer = Drawable.Transformer(surf, [FlagColorizer()])

        self.cloth.add(SpringSystem.ConstantAccelAffector, (0, 0, -0.04))
        self.cloth.add(SpringSystem.ClothWindAffector, surf, (-1, 0.001, 0.8))
        self.cloth.add(AnchorAffector)

    def getInitialState(self):
        xAxis = arange(0, 8, 0.5)
        yAxis = arange(5, 11, 0.5)
        size = (len(yAxis), len(xAxis))
        a = zeros(size + (3,), Float32, savespace=True)
        a[:,:,0] += 0 - xAxis
        a[:,:,2] += yAxis[:, NewAxis]
        return a

    def getTexCoords(self):
        xAxis = arange(0, 8, 0.5)
        yAxis = arange(5, 11, 0.5)
        size = (len(yAxis), len(xAxis))
        a = zeros(size + (2,), Float32, savespace=True)
        a[:,:,0] += xAxis
        a[:,:,1] += yAxis[:, NewAxis]
        return a

    def getDrawables(self):
        return [self.transformer] + self.pole.values()

    def update(self):
        # Use a fixed time step for now, since the model gets unstable
        # far too easily with a variable time step.
        self.cloth.integrate(0.03)
        

if __name__ == '__main__':
    loop = Event.EventLoop()
    viewport = Viewport.OpenGLViewport(loop, (800,600), 1000)
    view = ThreeDRender.View(viewport)
    control = ThreeDControl.Viewing(view, viewport)
    viewport.setCaption("Cloth Simulation Test")

    # Enzoomify the camera toward our object
    view.camera.position = (5,0,-5)
    view.camera.distance = 20
    view.camera.jump()

    # Give us some spiffy blue and yellow highlights
    glClearColor(0.3, 0.3, 0.3, 1)
    view.light0.ambient  = (0,0,0,1)
    view.light0.diffuse  = (0.5,0.5,0.4,1)
    view.light0.position = (0,200,200,1)
    view.light1.ambient  = (0,0,0,1)
    view.light1.diffuse  = (0.4,0.4,0.5,1)
    view.light1.position = (0,200,200,1)

    obj = Flag()
    viewport.onSetupFrame.observe(obj.update)
    view.scene.add(obj)

    view.scene.preprocess()
    Util.showFrameRate(viewport)
    loop.run()

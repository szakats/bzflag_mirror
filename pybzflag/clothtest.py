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


class Flag:
    def __init__(self):
        # Note that the size and origin are in world coordinates along the XZ
        # plane, and that resolution is in the cloth array. The origin of the
        # cloth array is at the base of the flag on the pole, with the top
        # of the array running up the pole.
        self.size = (8,6)
        self.origin = (0,4.5)
        self.resolution = (12,16)
        
        self.pole = Drawable.VRML.load("flagpole.wrl")
        self.cloth = SpringSystem.Cloth(self.getInitialState())

        self.surf = Drawable.ArraySurface(self.cloth.state, self.getTexCoords())
        self.surf.render.static = False
        self.surf.render.textures = (Texture.load("water.jpeg"),)

#        self.cloth.add(SpringSystem.ConstantAccelAffector, (0, 0, -0.04))
#        self.cloth.add(SpringSystem.ClothWindAffector, self.surf, (1, 0.001, 0.3))
#        self.cloth.add(AnchorAffector)

    def getInitialState(self):
        def xcoord(x,y):
            return self.origin[0] + self.size[0] * y / float(self.resolution[1]-1)
        def zcoord(x,y):
            return self.origin[1] + self.size[1] * x / float(self.resolution[0]-1)
        a = zeros(self.resolution + (3,), Float)
        a[:,:,0] = fromfunction(xcoord, self.resolution)
        a[:,:,2] = fromfunction(zcoord, self.resolution)
        return a

    def getTexCoords(self):
        def xf(x,y):
            return x / float(self.resolution[0]-1)
        def yf(x,y):
            return y / float(self.resolution[1]-1)
        a = zeros(self.resolution + (2,), Float)
        a[:,:,0] = fromfunction(xf, self.resolution)
        a[:,:,1] = fromfunction(yf, self.resolution)
        return a.astype(Float32)

    def getDrawables(self):
        return [self.surf] + self.pole.values()

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
    view.camera.position = (-5,0,-5)
    view.camera.distance = 20
    view.camera.azimuth = 0
    view.camera.jump()

    # Give us some spiffy blue and yellow highlights
    glClearColor(0.3, 0.3, 0.3, 1)
    view.light0.ambient  = (0,0,0,1)
    view.light0.diffuse  = (0.5,0.5,0.4,1)
    view.light0.position = (0,-200,200,1)
    view.light1.ambient  = (0,0,0,1)
    view.light1.diffuse  = (0.4,0.4,0.5,1)
    view.light1.position = (0,-200,200,1)

    obj = Flag()
    viewport.onSetupFrame.observe(obj.update)
    view.scene.add(obj)

    view.scene.preprocess()
    Util.showFrameRate(viewport)
    loop.run()

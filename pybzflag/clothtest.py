#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable, SpringSystem, Texture
from BZFlag import Event, Util, Animated
from Numeric import *
from time import time
from OpenGL.GL import *
from BZFlag.Geometry import *


class Flag:
    def __init__(self):
        # Note that the size and origin are in world coordinates along the XZ
        # plane, and that resolution is in the cloth array. The origin of the
        # cloth array is at the base of the flag on the pole, with the top
        # of the array running up the pole.
        self.size = (8,6)
        self.origin = (0,4.5)
        self.resolution = (20,20)
        self.time = Animated.Timekeeper()
        
        self.pole = Drawable.VRML.load("flagpole.wrl")
        self.cloth = SpringSystem.Cloth(self.getInitialState())

        self.surf = Drawable.ArraySurface(self.cloth.state, self.getTexCoords())
        self.surf.render.static = False

        # The texture has little cutouts on the edge to make the flag look a little
        # less new, but the flag won't render with blending properly unless we
        # do z-sorting, which would be a gigantic pain.
        self.surf.render.textures = (Texture.load("flag.png"),)

        # Gravity, plus a small force in the other two axes to keep the springs from
        # lining up perfectly and standing end-on-end.
        self.cloth.add(SpringSystem.ConstantAccelAffector, (0.001, 0.001, -0.04))

        # Wind!
        self.cloth.add(SpringSystem.ClothWindAffector, self.surf, (1,0,0.2), 0.15, 0.15)

        # Pin the cloth to the flagpole
        self.cloth.add(SpringSystem.ClothAnchorAffector, (-1,0), (0,0))

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
        self.cloth.integrate(self.time.step())

if __name__ == '__main__':
    loop = Event.EventLoop()
    viewport = Viewport.OpenGLViewport(loop, (800,600), 1000)
    view = ThreeDRender.View(viewport)
    control = ThreeDControl.Viewing(view, viewport)
    viewport.setCaption("Cloth Simulation Test")

    # Enzoomify the camera toward our object
    view.camera.position = (5,0,5)
    view.camera.distance = 20
    view.camera.jump()

    # Set up some lighting more appropriate for this test
    viewport.clearColor  = (0.3, 0.3, 0.3, 1)
    view.resetLighting()
    view.lights[0].enabled  = True
    view.lights[0].diffuse  = (1, 1, 0.98, 1)
    view.lights[0].position = (0,-200,200,1)

    obj = Flag()
    viewport.onSetupFrame.observe(obj.update)
    view.scene.add(obj)

    view.scene.preprocess()
    Util.showFrameRate(viewport)
    loop.run()

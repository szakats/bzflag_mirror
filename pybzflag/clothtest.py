#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable, SpringSystem, Texture
from BZFlag import Event, Util, Animated
from Numeric import *
from time import time
from OpenGL.GL import *
from BZFlag.Geometry import *


class Wind:
    """Global object for simulating wind. Can also be used as a scene
       object to draw the current wind state for debuggative purposes.
       """
    def __init__(self):
        self.drawables = None
        
        # Wind, with speed and direction varying over time with a perlin noise function
        self.vector = Animated.Vector(Animated.PerlinNoise(persistence = 0.8,
                                                           octaves     = 7,
                                                           amplitude   = 0.8,
                                                           frequency   = 0.01,
                                                           ))
        
    def integrate(self, dt):
        self.vector.integrate(dt)

    def getDrawables(self):
        if not self.drawables:
            self.drawables = [Drawable.Vector(self.vector)]
        return self.drawables
        

class Flag:
    """Scene object simulating one flag"""
    def __init__(self, wind):
        # Note that the size and origin are in world coordinates along the XZ
        # plane, and that resolution is in the cloth array. The origin of the
        # cloth array is at the base of the flag on the pole, with the top
        # of the array running up the pole.
        self.size = (8,6)
        self.origin = (0,4.5)
        self.resolution = (20,20)
        
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

        # Pin the cloth to the flagpole
        self.cloth.add(SpringSystem.ClothAnchorAffector, (-1,0), (0,0))

        # Now add the global wind we were given
        self.cloth.add(SpringSystem.ClothWindAffector, self.surf, wind.vector)

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

    def integrate(self, dt):
        self.cloth.integrate(dt)


if __name__ == '__main__':
    loop = Event.EventLoop()
    viewport = Viewport.OpenGLViewport(loop, (800,600), 1000)
    view = ThreeDRender.View(viewport)
    control = ThreeDControl.Viewing(view, viewport)
    viewport.setCaption("Cloth Simulation Test")

    # Enzoomify the camera toward our object
    view.camera.position = (0,0,5)
    view.camera.distance = 20
    view.camera.jump()

    # Set up some lighting more appropriate for this test
    viewport.clearColor  = (0.3, 0.3, 0.3, 1)
    view.resetLighting()
    view.lights[0].enabled  = True
    view.lights[0].diffuse  = (1, 1, 0.98, 1)
    view.lights[0].position = (0,-200,200,1)

    # Create our simulation objects
    time = Animated.Timekeeper()
    wind = Wind()
    flag = Flag(wind)
    view.scene.add(wind)
    view.scene.add(flag)

    # Update the simulation each frame
    def update():
        dt = time.step()
        wind.integrate(dt)
        flag.integrate(dt)
    viewport.onSetupFrame.observe(update)

    view.scene.preprocess()
    Util.showFrameRate(viewport)
    loop.run()

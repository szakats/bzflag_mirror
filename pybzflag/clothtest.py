#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable, SpringSystem
from BZFlag import Event, Util, Animated
from Numeric import *
from time import time


class ClothObject:
    def __init__(self):
        self.cloth = SpringSystem.Cloth(self.getInitialState())
        self.cloth.add(SpringSystem.GravityAffector)

        self.time = Animated.Timekeeper()
        self.drawables = [Drawable.ArraySurface(self.cloth.state)]
        self.drawables[0].render.static = False

    def getInitialState(self):
        xAxis = arange(-2, 2, 0.2)
        yAxis = arange( 0, 2, 0.2)
        size = (len(yAxis), len(xAxis))
        a = zeros(size + (3,), Float)
        a[:,:,0] += 0 - xAxis
        a[:,:,2] += yAxis[:, NewAxis]
        return a

    def getDrawables(self):
        return self.drawables

    def update(self):
        self.cloth.integrate(self.time.step())
        

if __name__ == '__main__':
    loop = Event.EventLoop()
    viewport = Viewport.OpenGLViewport(loop, (800,600), 1000)
    view = ThreeDRender.View(viewport)
    control = ThreeDControl.Viewing(view, viewport)

    # Enzoomify the camera toward our object
    view.camera.position = (0,0,0)
    view.camera.distance = 10
    view.camera.jump()

    # Give us some spiffy blue and yellow highlights
    view.light0.ambient  = (0,0,0,1)
    view.light0.diffuse  = (1,1,0.9,1)
    view.light0.position = (200,20,20,1)
    view.light1.ambient  = (0,0,0,1)
    view.light1.diffuse  = (0.8,0.8,1,1)
    view.light1.position = (20,200,20,1)

    obj = ClothObject()
    viewport.onSetupFrame.observe(obj.update)
    view.scene.add(obj)
    
    view.scene.preprocess()
    Util.showFrameRate(viewport)
    loop.run()

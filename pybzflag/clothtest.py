#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable
from BZFlag import Event, Util
from Numeric import *
from time import time


def buildArray():
    xAxis = arange(-5, 5, 0.2)
    yAxis = arange(-5, 5, 0.2)
    size = (len(xAxis), len(yAxis))
    a = zeros(size + (3,), Float)
    a[:,:,0] += xAxis
    a[:,:,1] += yAxis[:, NewAxis]
    return a


def updateArray(a):
    t = time()
    size = a.shape[:2]
    def f(x,y):
        x -= size[0]/2
        y -= size[1]/2
        d = sqrt(x*x + y*y)
        return 0.5 * cos(d/3 - t*8) * exp(-d/100)
    a[:,:,2] = fromfunction(f, size)
    
    
class TestObject:
    def __init__(self, drawables):
        self.drawables = drawables
    def getDrawables(self):
        return self.drawables


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
    view.light0.position = (-200,-20,-20,1)
    view.light1.ambient  = (0,0,0,1)
    view.light1.diffuse  = (0.8,0.8,1,1)
    view.light1.position = (-20,-200,-20,1)

    a = buildArray()
    surf = Drawable.ArraySurface(a)
    surf.render.static = False
    obj = TestObject([surf])

    def setupFrame():
        updateArray(a)
    viewport.onSetupFrame.observe(setupFrame)

    view.scene.add(obj)
    view.scene.preprocess()
    Util.showFrameRate(viewport)
    loop.run()

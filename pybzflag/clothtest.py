#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Drawable
from BZFlag import Event
from OpenGL.GL import *
from Numeric import *


def testArray2D(size):
    def f(x,y):
        x -= size[0]/2
        y -= size[1]/2
        d = sqrt(x*x + y*y)
        return 0.7 * cos(d/3) * exp(-d/20)
    return fromfunction(f, size)


def testArray3D():
    xAxis = arange(-2, 2, 0.05)
    yAxis = arange(-2, 2, 0.05)
    size = (len(xAxis), len(yAxis))
    zAxis = testArray2D(size)
    a = zeros(size + (3,), Float)
    a[:,:,0] += xAxis
    a[:,:,1] += reshape(repeat(yAxis, len(xAxis)), size)
    a[:,:,2] += zAxis
    return a
    
    
class ArrayMesh(Drawable.GLDrawable):
    def __init__(self, vertices):
        Drawable.GLDrawable.__init__(self)

        # Flatten the vertex array
        self.vertices = reshape(vertices, (-1, 3))

        # Precalculate a list of triangle vertex indices
        indices = []
        for x in xrange(vertices.shape[0] - 1):
            for y in xrange(vertices.shape[1] - 1):
                indices.append(x + y * vertices.shape[0])
                indices.append((x+1) + y * vertices.shape[0])
                indices.append(x + (y+1) * vertices.shape[0])
                indices.append(x + (y+1) * vertices.shape[0])
                indices.append((x+1) + y * vertices.shape[0])
                indices.append((x+1) + (y+1) * vertices.shape[0])
        self.indices = array(indices)
        
    def draw(self, rstate):
        glVertexPointerd(self.vertices)
        glEnable(GL_VERTEX_ARRAY)
        glDrawElementsui(GL_TRIANGLES, self.indices)
        glDisable(GL_VERTEX_ARRAY)


class TestObject:
    def __init__(self, drawables):
        self.drawables = drawables
    def getDrawables(self):
        return self.drawables




if __name__ == '__main__':
    loop = Event.EventLoop()
    viewport = Viewport.OpenGLViewport(loop, (800,600))
    view = ThreeDRender.View(viewport)
    control = ThreeDControl.Viewing(view, viewport)
    
    view.camera.position = (0,0,0)
    view.camera.distance = 10
    view.camera.jump()

    a = testArray3D()
    view.scene.add(TestObject([ArrayMesh(a)]))
    view.scene.preprocess()
    loop.run()

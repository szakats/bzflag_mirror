#!/usr/bin/env python
from BZFlag.UI import Viewport, Texture
from BZFlag.Event import EventLoop
from BZFlag import Animated, Util, Noise
from OpenGL.GL import *
from Numeric import *
import time

class NoiseTexture(Texture.Texture):
    """A texture filled with unfiltered random noise"""
    def __init__(self, size=(256,256), seed=None):
        Texture.Texture.__init__(self)
        r = Noise.randomArray(size, seed, (0,255), UInt8)
        self.loadRaw(size, r.tostring(), GL_LUMINANCE, 1)
        self.setFilter(GL_NEAREST, GL_NEAREST)


class NoiseRenderer:
    """Renders a NoiseTexture with scaling and rotation to
       generate a stream of random frames from one source texture.

       texture - The texture to scale and rotate. Generally this should be a
                 NoiseTexture instance with custom settings.

       scale - Texture coordinate scale

       rotationDistance - The distance away from the origin to put the center of rotation

       rotationSpeed - Number of degrees to advance the rotation every time a frame is generated
       """
    def __init__(self, texture=None, scale=50, rotationDistance=100, rotationSpeed=129):
        if not texture:
            texture = NoiseTexture()
        self.texture = texture
        self.scale = scale
        self.rotationDistance = rotationDistance
        self.rotationSpeed = rotationSpeed
        self.angle = 0

    def render(self, texture, size):
        """Render noise into the given texture. This assumes an ortho mode viewport"""

        # Prepare our texture
        glEnable(GL_TEXTURE_2D)
        glDisable(GL_LIGHTING)
        glDisable(GL_BLEND)
        glColor3f(1,1,1)
        self.texture.bind()
        
        # Rotate the texture matrix
        glMatrixMode(GL_TEXTURE)
        glLoadIdentity()
        self.angle += self.rotationSpeed
        glRotatef(self.angle, 0,0,1)
        glTranslatef(-self.rotationDistance, -self.rotationDistance, 0)
        glMatrixMode(GL_MODELVIEW)

        # Draw a unit square
        glBegin(GL_QUADS)
        glTexCoord2f(0,0)
        glVertex2f(0,0)
        glTexCoord2f(self.scale,0)
        glVertex2f(size[0],0)
        glTexCoord2f(self.scale,self.scale)
        glVertex2f(*size)
        glTexCoord2f(0,self.scale)
        glVertex2f(0,size[1])
        glEnd()

        # Clean up
        glMatrixMode(GL_TEXTURE)
        glLoadIdentity()
        glMatrixMode(GL_MODELVIEW)
        glDisable(GL_TEXTURE_2D)

        # Capture our rendered image
        texture.loadBackbuffer(size)


if __name__ == '__main__':
    loop = EventLoop()
    viewport = Viewport.OpenGLViewport(loop, (640,480), 1000)
    viewport.setCaption("OpenGL Noise Experiment")
    viewport.fov = None   

    r = NoiseRenderer()
    octaves = [Texture.Texture() for i in xrange(9)]

    def drawFrame():
        r.render(octaves[8], (2,2))
        r.render(octaves[7], (4,4))        
        r.render(octaves[6], (8,8))        
        r.render(octaves[5], (16,16))        
        r.render(octaves[4], (32,32))        
        r.render(octaves[3], (64,64))        
        r.render(octaves[2], (128,128))        
        r.render(octaves[1], (256,256))
        r.render(octaves[0], (512,512))
        
        glEnable(GL_TEXTURE_2D)
        glEnable(GL_BLEND)
        glColor4f(1,1,1,0.5)

        for octave in octaves:
            octave.bind()
            
            glBegin(GL_QUADS)
            glTexCoord2f(0,0)
            glVertex2f(0,0)
            glTexCoord2f(1,0)
            glVertex2f(viewport.size[0],0)
            glTexCoord2f(1,1)
            glVertex2f(*viewport.size)
            glTexCoord2f(0,1)
            glVertex2f(0,viewport.size[1])
            glEnd()

    Util.showFrameRate(viewport)
    viewport.onDrawFrame.observe(drawFrame)
    loop.run()

#!/usr/bin/env python
#
# Test rig for our GLText module
#
from BZFlag.UI import Viewport, GLText
from BZFlag.Event import EventLoop

# Create a new orthogonal mode viewport
loop = EventLoop()
#viewport = Viewport.OpenGLViewport(loop, (640,480))
viewport = Viewport.PygameViewport(loop, (640,480))
viewport.setCaption("Font Test")
viewport.fov = None   

f = GLText.Font("VeraBd.ttf")

def drawFrame():
    viewport.screen.blit(f.sizes[20].pages[0].surface, (0,0))
    #f.draw("Boing")

viewport.onDrawFrame.observe(drawFrame)
loop.run()



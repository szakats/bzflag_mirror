#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Environment, Drawable
from BZFlag import Event, Util

# Viewport, view, and event loop setup
loop = Event.EventLoop()
viewport = Viewport.OpenGLViewport(loop, (800,600), 1000)
view = ThreeDRender.View(viewport)
control = ThreeDControl.Viewing(view, viewport)
viewport.setCaption("Sky Test")

# Move the camera
view.camera.position = (0,0,0)
view.camera.distance = 400
view.camera.elevation = 0
view.camera.jump()

# Add the sky
sky = Environment.Sky()
view.scene.add(sky)
viewport.onSetupFrame.observe(sky.update)

# Add a navigation widget so we don't get too disoriented
class Axes:
    def __init__(self):
        self.drawables = Drawable.VRML.load("nav_widget.wrl").values()
    def getDrawables(self):
        return self.drawables
view.scene.add(Axes())

Util.showFrameRate(viewport)
loop.run()

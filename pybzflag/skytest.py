#!/usr/bin/env python
from BZFlag.UI import Viewport, ThreeDRender, ThreeDControl, Environment
from BZFlag import Event, Util

loop = Event.EventLoop()
viewport = Viewport.OpenGLViewport(loop, (800,600), 1000)
view = ThreeDRender.View(viewport)
control = ThreeDControl.Viewing(view, viewport)
viewport.setCaption("Sky Test")

sky = Environment.Sky()

view.scene.add(sky)
viewport.onSetupFrame.observe(sky.update)

view.scene.preprocess()
Util.showFrameRate(viewport)
loop.run()

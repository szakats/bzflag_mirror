#!/usr/bin/env python
#
# Test for perlin-noise-based mountain ranges
#
from __future__ import division
from BZFlag.UI import Viewport
from BZFlag import Event, Util, Noise
from Numeric import *
import pygame

loop = Event.EventLoop()
viewport = Viewport.PygameViewport(loop, (512,64), resizable=False)
n = Noise.PerlinNoise(1, octaves=12, persistence=0.4)

viewport.screen.fill((0,0,0))

width = 40
heights = n.get(arange(0, width, width/viewport.size[0])[...,NewAxis])

for x in xrange(viewport.size[0]):
    height = heights[x] * viewport.size[1]/2 + viewport.size[1]/2
    viewport.screen.fill((255,255,255),
                         (x,viewport.size[1] - height,1,height))

loop.run()

""" BZFlag.UI.ThreeDRender

An abstract base view that provides management for rendering passes,
viewports, cameras, lighting, and picking. This should be subclassed
to implement views that need moderately complex 3D engine behaviour.
"""
#
# Python BZFlag Protocol Package
# Copyright (C) 2003 Micah Dowty <micahjd@users.sourceforge.net>
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

import pygame
from pygame.locals import *
from BZFlag.UI import Texture, Drawable
from BZFlag import Event, Animated
from OpenGL.GL import *
from OpenGL.GLU import *
import sys


class Camera:
    """Abstraction for the OpenGL transforms used to set up the camera.
       For 1st person operation, use 'position', 'azimuth' and 'elevation',
       with 'distance' set to zero. For 3rd person operation, set 'distance'
       to a nonzero value.
       """
    def __init__(self):
        self.position  = (0, 0, 0)
        self.distance  = 0
        self.azimuth   = 0
        self.elevation = 0
	self.azimuthOffset = 0

    def jump(self):
        """If this camera class implements any animation techniques, this method
           should ensure that the camera arrives instantly at its destination after
           the coordinates have been changed.
           """
        pass

    def load(self):
        """Set the current OpenGL matrix according to the camera's location and orientation"""
        glLoadIdentity()
        glTranslatef(0, 0, -self.distance)
        glRotatef(self.elevation, 1.0, 0.0, 0.0)
        glRotatef(self.azimuth + self.azimuthOffset, 0.0, 0.0, 1.0)
        glTranslatef(*self.position)


class SmoothedCamera(Camera):
    """Replacement Camera class that smooths all axes with the Animated module"""
    def __init__(self):
        Camera.__init__(self)
        self.time = Animated.Timekeeper()
        self.animatedDistance  = Animated.Value (self.distance,
                                                 Animated.LogApproach(self.distance, 4))
        self.animatedPosition  = Animated.Vector(self.position,
                                                 Animated.LogApproach(self.position, 10))
        self.animatedAzimuth   = Animated.Value (self.azimuth,
                                                 Animated.LogApproach(self.azimuth, 10))
        self.animatedElevation = Animated.Value (self.elevation,
                                                 Animated.LogApproach(self.elevation, 10))
        self.animated = [self.animatedDistance,
                         self.animatedPosition,
                         self.animatedAzimuth,
                         self.animatedElevation]

    def jump(self):
        """This cuts directly to the target position, without all that pesky animation"""
        self.animatedDistance.value  = self.distance
        self.animatedPosition.set(self.position)
        self.animatedAzimuth.value = self.azimuth
        self.animatedElevation.value = self.elevation

    def load(self):
        self.animatedDistance.f.target  = self.distance
        self.animatedPosition.f.target  = self.position
        self.animatedAzimuth.f.target   = self.azimuth
        self.animatedElevation.f.target = self.elevation
        dt = self.time.step()
        for item in self.animated:
            item.integrate(dt)
        glLoadIdentity()
        glTranslatef(0, 0, -self.animatedDistance.value)
        glRotatef(self.animatedElevation.value, 1.0, 0.0, 0.0)
        glRotatef(self.animatedAzimuth.value + self.azimuthOffset, 0.0, 0.0, 1.0)
        glTranslatef(*self.animatedPosition.get())


class Light:
    """Abstraction for an OpenGL light's position and intensity"""
    def __init__(self, lightnum):
        self.ambient  = (0, 0, 0, 1.0)
        self.diffuse  = (0, 0, 0, 1.0)
        self.position = (0, 0, 0, 1.0)
        self.lightnum = lightnum

    def set(self):
        """Set up and enable the light in the current OpenGL context"""
        glLightfv(self.lightnum, GL_AMBIENT, self.ambient)
        glLightfv(self.lightnum, GL_DIFFUSE, self.diffuse)
        glLightfv(self.lightnum, GL_POSITION, self.position)
        glEnable(self.lightnum)


class TextureGroup(Drawable.DisplayList):
    """A container for all the objects drawn in the same texture, compiled
       into a display list for speedy rendering of static worlds.
       """
    def __init__(self, drawables=[]):
        self.drawables = drawables
        Drawable.DisplayList.__init__(self)

    def init(self):
        """Don't rebuild the list on init"""
        pass

    def drawToList(self):
        for drawable in self.drawables:
            if hasattr(drawable, 'drawToList'):
                drawable.drawToList()
            else:
                drawable.draw()


class RenderPass:
    """A rendering pass defines the highest level of grouping for drawables.
       Rendering passes should separate blended and unblended objects, or
       groups of objects that require expensive OpenGL state changes.
       This is an abstract base class.
       """
    # Rendering passes with a higher priority get a chance to filter drawables first
    priority = None

    def __init__(self):
        self.erase()

    def render(self, names = None, curname = None):
        pass

    def filter(self, drawable):
        """Determine whether the given drawable belongs in this
           pass, return True or False
           """
        pass

    def preprocess(self):
        """This is called after one or more objects are modified, to perform
           any necessary preprocessing on the render pass before rendering.
           """
        pass

    def add(self, drawable):
        pass

    def erase(self):
        """Remove all drawables from this pass"""
        pass


class BasicRenderPass(RenderPass):
    """A simple render pass for the majority of objects. Sorts objects by texture.
       This is the lowest priority render pass, and will collect all drawables
       left over from other passes.
       """
    filterPriority = 0
    renderPriority = 100

    def filter(self, drawable):
        return True

    def erase(self):
        self.textureGroups = {}

    def add(self, drawable):
        """Files the given drawable according to texture group"""
        groups = self.textureGroups
        if groups.has_key(drawable.texture):
            groups[drawable.texture].drawables.append(drawable)
        else:
            groups[drawable.texture] = TextureGroup([drawable])

    def preprocess(self):
        """This builds display lists for all our texture groups"""
        for texgroup in self.textureGroups.itervalues():
            texgroup.buildList()

    def render(self, names = None, curname = None):
        """Perform one render pass- iterates through each texture group in the pass,
           binding the pass' texture and drawing the texture group.
           """
        if names == None:
            for (texture, group) in self.textureGroups.iteritems():
                if texture is None:
                    glDisable(GL_TEXTURE_2D)
                else:
                    glEnable(GL_TEXTURE_2D)
                    texture.bind()
                group.draw()
        else:
            for (texture, group) in self.textureGroups.iteritems():
                for drawable in group.drawables:
                    names[curname] = drawable
                    glLoadName(curname)
                    drawable.draw()
                    curname += 1


class BlendedRenderPass(BasicRenderPass):
    """A rendering pass that collects blended objects and renders them after most other objects."""
    filterPriority = 10
    renderPriority = 90

    def filter(self, drawable):
        return drawable.blended

    def render(self, names = None, curname = None):
        glEnable(GL_BLEND)
        BasicRenderPass.render(self, names, curname)
        glDisable(GL_BLEND)


class OverlayRenderPass(BasicRenderPass):
    """A rendering pass that draws objects overlaid on the current scene."""
    filterPriority = 20
    renderPriority = 80

    def filter(self, drawable):
        return drawable.overlay

    def render(self, names = None, curname = None):
        glClear(GL_DEPTH_BUFFER_BIT)
        BasicRenderPass.render(self, names, curname)


class Scene:
    """Set of all the objects this view sees, organized into rendering passes
       and sorted by texture. Multiple rendering passes are necessary to deal
       with blended objects, and texture sorting is an important OpenGL optimization.
       """
    def __init__(self):
        self.erase()
        self.passes = [BasicRenderPass(), BlendedRenderPass(), OverlayRenderPass()]

    def erase(self):
        self.objects = {}

    def add(self, object, drawables):
        """Add the given object and drawables to the scene."""
        for drawable in drawables:
            drawable.parent(object)
        self.objects.setdefault(object, []).extend(drawables)

    def preprocess(self):
        """Rebuilds rendering passes. Currently this is necessary when the world changes."""
        # Sort the rendering passes by decreasing render priority
        self.passes.sort(lambda a,b: cmp(b.renderPriority, a.renderPriority))

        # Make a list of rendering passes sorted by filter priority
        filterPasses = self.passes[:]
        filterPasses.sort(lambda a,b: cmp(b.filterPriority, a.filterPriority))

        # Clean out each rendering pass
        for rpass in self.passes:
            rpass.erase()

        # Divy up the drawables into rendering passes using the passes' filter functions
        for object, drawables in self.objects.iteritems():
            for drawable in drawables:
                for rpass in filterPasses:
                    if rpass.filter(drawable):
                        rpass.add(drawable)
                        break

        # Give each rendering pass a chance to preprocess
        for rpass in self.passes:
            rpass.preprocess()

    def render(self):
        """Render the scene to the current OpenGL context"""
        glDisable(GL_BLEND)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_NORMALIZE)
        glEnable(GL_CULL_FACE)
        glEnable(GL_COLOR_MATERIAL)
	glEnable(GL_LIGHTING)
        glDisable(GL_LINE_SMOOTH)
        glColor4f(1,1,1,1)

        for rpass in self.passes:
            rpass.render()

    def pick(self, viewport, pos):
        """Returns the nearest scene object that was rendered at the given screen
           coordinates. If no object was rendered at this postion, returns None.
           """
        # Flip the Y axis to convert from the traditional top-left-based coordinate
        # system to OpenGL's bottom-left-based coordinate system.
        pos = (pos[0], viewport.size[1] - pos[1])

        glSelectBuffer(len(self.objects) * 4)
        glRenderMode(GL_SELECT)
        glInitNames()
        glPushName(0)

        glMatrixMode(GL_PROJECTION)
        glPushMatrix()
        glLoadIdentity()
        gluPickMatrix(pos[0], pos[1], 5, 5, viewport.rect)
        viewport.setProjectionMatrix()
        glMatrixMode(GL_MODELVIEW)

        names = {}
        curname = 1
        for rpass in self.passes:
            rpass.render(names, curname)

        glMatrixMode(GL_PROJECTION)
        glPopMatrix()
        glMatrixMode(GL_MODELVIEW)

        hits = glRenderMode(GL_RENDER)
        if len(hits) > 0:
            depth = hits[0][1]
            choose = hits[0][2]
            for hit in hits:
                if hit[1] < depth:
                    depth = hit[1]
                    choose = hit[2]
            return names[choose[0]].object
        return None

class View:
    """A generic 3D view with multiple rendering passes, usable with OpenGLViewport."""
    def __init__(self, viewport, scene=None):
        if not scene:
            scene = Scene()
        self.viewport = viewport
        self.camera = Camera()
        self.scene = scene
        viewport.onDrawFrame.observe(self.render)
        self.initLighting()

    def initLighting(self):
        self.light0 = Light(GL_LIGHT0)
        self.light1 = Light(GL_LIGHT1)
        self.light0.ambient  = (0.05, 0.05, 0.05, 1.0)
        self.light0.diffuse  = (0.85, 0.85, 0.85, 1.0)
        self.light0.position = (400, 400, 400, 1.0)
        self.light1.ambient  = (0.05, 0.05, 0.05, 1.0)
        self.light1.diffuse  = (0.85, 0.85, 0.85, 1.0)
        self.light1.position = (0, 0, 400, 1.0)
        self.lights = [self.light0, self.light1]

    def render(self):
        self.camera.load()
        # Set up the light so it is in world space not cam space
        for light in self.lights:
            light.set()
        self.scene.render()

    def pick(self, pos):
        self.camera.load()
        return self.scene.pick(self.viewport, pos)

### The End ###

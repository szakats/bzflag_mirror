""" BZFlag.UI.ThreeDView

A 3d scene renderer similar to BZFlag proper
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

import pygame, BZFlag
from pygame.locals import *
from BZFlag.World import WorldObjects
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
        self.position  = (0, 0, 4.0)
        self.distance  = 900
        self.azimuth   = 180
        self.elevation = -75
	self.azimuthOffset = 0

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
            print drawable
            if hasattr(drawable, 'drawToList'):
                drawable.drawToList()
            else:
                drawable.draw()

            
class Scene:
    """Set of all the objects this view sees, organized into rendering passes
       and sorted by texture. Multiple rendering passes are necessary to deal
       with blended objects, and texture sorting is an important OpenGL optimization.
       This class observes the current game state, making the necessary updates when
       the world is modified, players move, etc.
       """
    def __init__(self, game):
        self.game = game
        self.objects = {}
        self.passes = [{}, {}]
        game.world.onLoad.observe(self.reloadWorld)
        self.reloadWorld()

    def reloadWorld(self):
        """Rebuild the internal scene structures from the game's world"""
        for block in self.game.world.blocks:
            if isinstance(block, WorldObjects.WorldObject):
                self.objects[block] = block.getGLDrawables()
        self.rebuildTexmap()

    def rebuildTexmap(self):
        """Rebuilds the mapping from texture to object, used to sort objects
           by texture within each rendering pass.
           """
        for object, drawables in self.objects.items():
            for drawable in drawables:
                if drawable.blended:
                    if self.passes[1].has_key(drawable.texture):
                        self.passes[1][drawable.texture].drawables.append(drawable)
                    else:
                        self.passes[1][drawable.texture] = TextureGroup([drawable])
                else:
                    if self.passes[0].has_key(drawable.texture):
                        self.passes[0][drawable.texture].drawables.append(drawable)
                    else:
                        self.passes[0][drawable.texture] = TextureGroup([drawable])
        for p in self.passes:
            for texgroup in p.values():
                texgroup.buildList()
                
    def renderPass(self, p):
        """Perform one render pass- iterates through each texture group in the pass,
           binding the pass' texture and drawing the texture group.
           """
        for (texture, group) in p.items():
            if texture is None:
                glDisable(GL_TEXTURE_2D)
            else:
                glEnable(GL_TEXTURE_2D)
                texture.bind()
            group.draw()

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

        self.renderPass(self.passes[0])
        glEnable(GL_BLEND)
        self.renderPass(self.passes[0])


class ThreeDView:
    """Shows a 3D view of the BZFlag game, renderable to an OpenGLViewport.
       This can be used for observer views, 1st person game views, and editing.
       """
    def __init__(self, game, viewport):
        self.game = game
        self.viewport = viewport
        # Initialize the opengl view
        self.camera = Camera()
        self.scene = Scene(game)
        viewport.setCaption("%s 3D View" % BZFlag.name)
        def onDrawFrame():
            game.update()
            self.render()
        viewport.onDrawFrame.observe(onDrawFrame)
        self.light0 = Light(GL_LIGHT0)
        self.light1 = Light(GL_LIGHT1)
        self.light0.ambient  = (0.05, 0.05, 0.05, 1.0)
        self.light0.diffuse  = (0.85, 0.85, 0.85, 1.0)
        self.light0.position = (400, 400, 400, 1.0)
        self.light1.ambient  = (0.05, 0.05, 0.05, 1.0)
        self.light1.diffuse  = (0.85, 0.85, 0.85, 1.0)
        self.light1.position = (0, 0, 400, 1.0)

    def render(self):
        """Render the view to the given surface. This includes the game
           world, with transient objects such as players and flags
           """
        self.camera.load()
        # Set up the light so it is in world space not cam space
        self.light0.set()
        self.light1.set()
        self.scene.render()

    def pick(self, pos):
        """Returns the closest scene object that was rendered at the given screen
           coordinates. For normal BZFlag world objects, this will be an instance
           of one of the classes defined in BZFlag.Protocol.WorldObjects. If no
           object was rendered at this postion, returns None.
           """
        # Flip the Y axis to convert from the traditional top-left-based coordinate
        # system to OpenGL's bottom-left-based coordinate system.
        pos = (pos[0], self.viewport.size[1] - pos[1])

        glSelectBuffer(len(self.scene.objects) * 4)
        glRenderMode(GL_SELECT)
        glInitNames()
        glPushName(0)

        glMatrixMode(GL_PROJECTION)
	glPushMatrix()
        glLoadIdentity()
        gluPickMatrix(pos[0], pos[1], 5, 5, self.viewport.rect)
        self.viewport.setProjectionMatrix()
        glMatrixMode(GL_MODELVIEW)
        self.camera.load()

	names = {}
	curname = 1
        for object, drawables in self.scene.objects.items():
	    names[curname] = object
            glLoadName(curname)
            curname += 1
            for drawable in drawables:
                drawable.draw()

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
            return names[choose[0]]
	return None


def attach(game, eventLoop):
    from BZFlag.UI import Viewport, ThreeDControl
    viewport = Viewport.OpenGLViewport(eventLoop, (800,600))
    view = ThreeDView(game, viewport)
    ThreeDControl.Viewing(view, viewport)
    return viewport

### The End ###

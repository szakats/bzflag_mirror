""" BZFlag.UI.Drawable.Box

Classes to draw the boxes in the world
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

from __future__ import division
from DisplayList import *
from OpenGL.GL import *
from OpenGL.GL.ARB.multitexture import *
from BZFlag.UI import GLExtension
from BZFlag.World import Scale
import random


class Sides(DisplayList):
    """Abstract base class for box sides"""
    def set(self, box):
        self.center = box.center
        self.angle = box.angle
        self.size = box.size
        try:
            self.render.textures[1].texEnv = GL_MODULATE
        except IndexError:
            pass

    def drawToList(self):
        glPushMatrix()
        glTranslatef(*self.center)
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glBegin(GL_QUADS)
        # Y+ side
        glNormal3f(0, 1, 0)
        glTexCoord2f(0, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0)
        glVertex3f(-self.size[0], self.size[1], 0)
        glTexCoord2f(0, self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, self.tex2Repeats[2])
        glVertex3f(-self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[0], self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[0], self.tex2Repeats[2])
        glVertex3f(self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[0], 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[0], 0)
        glVertex3f(self.size[0], self.size[1], 0)
        # Y- side
        glNormal3f(0, -1, 0)
        glTexCoord2f(0, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        glTexCoord2f(0, self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, self.tex2Repeats[2])
        glVertex3f(self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[0], self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[0], self.tex2Repeats[2])
        glVertex3f(-self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[0], 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[0], 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        # X+ side
        glNormal3f(1, 0, 0)
        glTexCoord2f(0, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0)
        glVertex3f(self.size[0], self.size[1], 0)
        glTexCoord2f(0, self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, self.tex2Repeats[2])
        glVertex3f(self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[1], self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[1], self.tex2Repeats[2])
        glVertex3f(self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[1], 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[1], 0)
        glVertex3f(self.size[0], -self.size[1], 0)
        # X- side
        glNormal3f(-1, 0, 0)
        glTexCoord2f(0, 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0)
        glVertex3f(-self.size[0], -self.size[1], 0)
        glTexCoord2f(0, self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, self.tex2Repeats[2])
        glVertex3f(-self.size[0], -self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[1], self.texRepeats[2])
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[1], self.tex2Repeats[2])
        glVertex3f(-self.size[0], self.size[1], self.size[2])
        glTexCoord2f(self.texRepeats[1], 0)
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, self.tex2Repeats[1], 0)
        glVertex3f(-self.size[0], self.size[1], 0)
        glEnd()
        glPopMatrix()


class BrickSides(Sides):
    """Sides of a standard bzflag brick box"""
    textureNames = ("brick.jpeg", "wall_grime.jpeg")
    def __init__(self, box):
        # This is brick, so we want to make sure we don't end up
        # with a fractional number of bricks. This brick texture has
        # 5 bricks vertically and about 5 horizontally.
        numBricks = (int(box.size[0]),
                     int(box.size[1]),
                     int(box.size[2]))
        self.texRepeats = (numBricks[0] / 5,
                           numBricks[1] / 5,
                           numBricks[2] / 5)
        self.tex2Repeats = (box.size[0] / 20,
                            box.size[1] / 20,
                            1)
        Sides.__init__(self, box)


class PlatformSides(Sides):
    """Sides of a thin platform box. This makes platforms look like two
       thin pieces of concrete sandwiching some lighter concrete, with
       bolts holding it all together. Looks a bit better than a very thin
       section of brick.
       """
    textureNames = ("platform_side.jpeg", "wall_grime.jpeg")
    def __init__(self, box):
        aspect = 8/10
        self.texRepeats = (box.size[0] / box.size[2] * 2 * aspect,
                           box.size[1] / box.size[2] * 2 * aspect,
                           1)
        self.tex2Repeats = (box.size[0] / 4,
                            box.size[1] / 4,
                            1)    
        Sides.__init__(self, box)


class End(DisplayList):
    """Abstract base class for the top and bottom sides of a box"""
    textureName = 'concrete.jpeg'

    def drawToList(self):
        glPushMatrix()
        glTranslatef(0, 0, self.height)
        glNormal3f(0, 0, 1)
        glBegin(GL_POLYGON)
        for i in xrange(4):
            vertex = self.polygon[i]
            glTexCoord2f(vertex[0] / 20, vertex[1] / 20)
            glVertex2f(*vertex)
        glEnd()
        glPopMatrix()


class Top(End):
    def set(self, box):
        self.polygon = box.toPolygon()
        self.height = box.center[2] + box.size[2]


class Bottom(End):
    def set(self, box):
        self.polygon = box.toPolygon()
        self.polygon.reverse()
        self.height = box.center[2]


class TopDecal(DisplayList):
    """Abstract base class for decals covering the entire top of a box"""
    texCoords = ( (0,0), (1,0), (1,1), (0,1) )

    def set(self, box):
        self.center = box.center
        self.size = box.size
        self.angle = box.angle
        self.render.decal = True

    def drawToList(self):
        glPushMatrix()
        glNormal3f(0, 0, -1)
        glTranslatef(*self.center)
        glTranslatef(0, 0, self.size[2])
        glRotatef(self.angle, 0.0, 0.0, 1.0)

        glBegin(GL_QUADS)
        glTexCoord2f(*self.texCoords[0])
        glVertex2f(-self.size[0], -self.size[1])
        glTexCoord2f(*self.texCoords[1])
        glVertex2f( self.size[0], -self.size[1])
        glTexCoord2f(*self.texCoords[2])
        glVertex2f( self.size[0],  self.size[1])
        glTexCoord2f(*self.texCoords[3])
        glVertex2f(-self.size[0],  self.size[1])
        glEnd()
        glPopMatrix()


class OilStain(TopDecal):
    """A random oil stain decal"""
    def __init__(self, box):
        self.textureName = random.choice(('oilstain_1.png',
                                          'oilstain_2.png',
                                          'oilstain_3.png'))
        TopDecal.__init__(self, box)


class TreadStain(TopDecal):
    """A tread stain decal, aligned to the major axis of the box"""
    textureName = "treadstain_1.png"
    
    def __init__(self, box):
        if box.size[1] > box.size[0]:
            self.texCoords = ( (0,0), (0,1), (1,1), (1,0) )
        TopDecal.__init__(self, box)


def detectBoxDrawables(box):
    """Given a box WorldObject, return a list of the drawables that should be used
       to represent it. This looks at the size and placement of the box to determine
       which drawables to use to represent it.
       """
    sides = None
    top = None
    bottom = None
    drawables = []

    # Calculate a few values based on the box proportions to make the below decisions clearer
    height = box.size[2]
    majorAxis = max(box.size[0], box.size[1])
    minorAxis = min(box.size[0], box.size[1])
    axisRatio = majorAxis / minorAxis

    # If the box isn't on the ground and it's fairly flat, assume it's a platform
    if height < 8 and box.center[2] > 1:
        sides = PlatformSides(box)

    # If the box is fairly large and squareish, use a square oil-stain texture
    if minorAxis > 10 and axisRatio < 2:
        drawables.append(OilStain(box))

    # If the box is about the same width as a tank, put some tread marks on it
    if minorAxis > Scale.TankWidth and minorAxis < Scale.TankWidth * 4 and majorAxis > Scale.TankWidth * 4:
        drawables.append(TreadStain(box))

    # Pick defaults for any characteristics we haven't decided on already and return a list
    if sides:
        drawables.append(sides)
    else:
        drawables.append(BrickSides(box))
    if top:
        drawables.append(top)
    else:
        drawables.append(Top(box))
    if bottom:
        drawables.append(bottom)
    else:
        drawables.append(Bottom(box))
    return drawables

### The End ###

""" BZFlag.UI.Texture

Texture loading utilities
"""
#
# Python BZFlag Package
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
import Image, pygame
from OpenGL.GL import *
from OpenGL.GLU import *
from BZFlag import Util, Animated
from BZFlag.UI import GLExtension
from OpenGL.GL.EXT.texture_filter_anisotropic import *
import copy, math, random


class Texture:
    """Represents an OpenGL texture, optionally loaded from disk in any format supported by PIL"""
    target = GL_TEXTURE_2D

    def __init__(self, name=None):
        self.texture = glGenTextures(1)
        self.texEnv = GL_MODULATE
        self.setDefaults()
        if name:
            self.loadFile(name)

    def loadFile(self, name):
        """Load the texture from disk, using PIL to open the file"""
        self.loadImage(Image.open(name))

    def loadImage(self, image):
        """Load the texture from a PIL image"""
        if image.mode == "RGBA":
            string = image.tostring('raw', 'RGBA', 0, -1)
            self.loadRaw(image.size, string, GL_RGBA, 4)
        elif image.mode == "RGB":
            string = image.tostring('raw', 'RGB', 0, -1)
            self.loadRaw(image.size, string, GL_RGB, 3)
        elif image.mode == "L":
            string = image.tostring('raw', 'L', 0, -1)
            self.loadRaw(image.size, string, GL_LUMINANCE, 1)
        else:
            raise Exception("Unsupported image mode '%s'" % image.mode)

    def loadSurface(self, surface, monochrome=False):
        """Load the texture from a pygame surface"""
        if monochrome:
            # pygame doesn't support monochrome, so the fastest way
            # appears to be using PIL to do the conversion.
            string = pygame.image.tostring(surface, "RGB")
            image = Image.fromstring("RGB", surface.get_size(), string).convert("L")
            string = image.tostring('raw', 'L', 0, -1)
            self.loadRaw(surface.get_size(), string, GL_LUMINANCE, GL_INTENSITY8)
        else:
            string = pygame.image.tostring(surface, "RGB", True)
            self.loadRaw(surface.get_size(), string, GL_RGB, 3)

    def loadRaw(self, size, string, format, components):
        """Load a raw image from the given string. 'format' is a constant such as
           GL_RGB or GL_RGBA that can be passed to gluBuild2DMipmaps.
           """
        self.size = size
        self.string = string
        self.format = format
        self.components = components
        (w,h) = size
        Texture.bind(self)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
        gluBuild2DMipmaps(self.target, components, w, h, format, GL_UNSIGNED_BYTE, string)

    def loadBackbuffer(self, size, position=(0,0), format=GL_RGB):
        """Load this texture from the given location on the backbuffer. This accepts
           coordinates in PyBZFlag style rather than OpenGL style- the origin is at the top-left
           corner of the viewport.
           """
        self.size = size
        self.format = format
        glReadBuffer(GL_BACK)
        Texture.bind(self)
        viewport = glGetIntegerv(GL_VIEWPORT)
        glCopyTexImage2D(self.target, 0, format,
                         position[0], viewport[3] - position[1] - size[1],
                         size[0], size[1],
                         0)

        # Disable mipmapping, since we're not generating any mipmap levels
        self.setFilter(GL_LINEAR, GL_LINEAR)

    def setDefaults(self):
        """Set the default OpenGL options for this texture"""
        self.setRepeat()
        self.setFilter()
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
        if GLExtension.maxAnisotropy > 1:
            glTexParameterf(self.target, GL_TEXTURE_MAX_ANISOTROPY_EXT, GLExtension.maxAnisotropy)

    def setRepeat(self, u=GL_REPEAT, v=GL_REPEAT):
        Texture.bind(self)
        glTexParameteri(self.target, GL_TEXTURE_WRAP_S, u)
        glTexParameteri(self.target, GL_TEXTURE_WRAP_T, v)

    def setFilter(self, min=GL_LINEAR_MIPMAP_LINEAR, mag=GL_LINEAR):
        Texture.bind(self)
        glTexParameteri(self.target, GL_TEXTURE_MIN_FILTER, min)
        glTexParameteri(self.target, GL_TEXTURE_MAG_FILTER, mag)

    def __getstate__(self):
        """This method is called when pickling a Texture, to return an object which is
           stored then passed to __setstate__ on unpickling.
           """
        return (self.size, self.string, self.format, self.components)

    def __setstate__(self, state):
        """This method is called on unpickling a Texture"""
        self.loadRaw(*state)

    def __getinitargs__(self):
        """This is here to force __init__ to be called on unpickling, to get us a new texture ID"""
        return ()

    def __del__(self):
        try:
            glDeleteTextures(self.texture)
        except:
            pass

    def bind(self, rstate=None, target=None):
        """Bind this texture to self.target in the current OpenGL context"""
        if not target:
            target = self.target
        glBindTexture(target, self.texture)
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, self.texEnv)


class AnimatedTexture:
    """Load a sequence of still images for an animated texture.
       Animated textures have a printf-style format specifier to
       insert a frame number, and end with a colon and a frame rate.
       For example, radar%02d.png:15
       """
    def __init__(self, name):
        (filename, framerate) = name.split(":")
        framerate = float(framerate)

        # Try to load texture frames. An error on the
        # first frame is fatal, but an error after that simply stops the loading.
        self.frames = []
        try:
            frameNumber = 0
            while True:
                self.frames.append(Texture(filename % frameNumber))
                frameNumber += 1
        except IOError:
            if not self.frames:
                raise

        self.frameNumber = Animated.Value(Animated.RampFunction(
            1/framerate * len(self.frames), (0, len(self.frames))))
        self.time = Animated.Timekeeper()
        self.target = GL_TEXTURE_2D

    def bind(self, rstate=None):
        self.frameNumber.integrate(self.time.step())
        self.frames[int(self.frameNumber.value)].bind(rstate)


class DynamicTextureException(Exception):
    pass


class DynamicTexture(Texture):
    """Abstract base class for a texture which is captured from an OpenGL rendering.
       The size given is treated as a preferred size- the actual texture size may
       be smaller due to viewport size limitations in the current video mode.

       By default the generated texture does not expire unless the 'dirty' flag
       is explicitly set. This can be changed by providing a meanExpiration time
       in seconds. To randomly vary the expiration time, expStdDeviation can be
       set to a nonzero value. Expiration times are generated according to a normal
       distribution.
       """
    def __init__(self, maxSize, meanExpiration=None, expStdDeviation=0):
        Texture.__init__(self)
        self.meanExpiration = meanExpiration
        self.expStdDeviation = expStdDeviation
        self.maxSize = maxSize
        self.viewport = None
        self.setDirty()
        self.rendered = False
        self.rstate = None
        self.renderLifetime = 0
        self.time = Animated.Timekeeper()
        self.format = GL_RGB
        self.dependencies = []
        self.newExpiration()

    def addDependencies(self, *textures):
        """Register the given texture as a dependency of this one. It will be
           rendered before this one starts rendering if necessary.
           """
        self.dependencies.extend(textures)

    def setDirty(self):
        """Call this to set the dirty flag, and cause the texture to be
           regenerated during the next frame.
           """
        self.dirty = True
        if self.viewport:
            self.viewport.show()

    def newExpiration(self):
        """Create a new random expiration duration"""
        if self.meanExpiration:
            if self.expStdDeviation:
                self.expiration = random.normalvariate(self.meanExpiration, self.expStdDeviation)
            else:
                self.expiration = self.meanExpiration
        else:
            self.expiration = None

    def bind(self, rstate=None):
        """The first time we're bound we add a new viewport that renders our texture"""
        if not self.rendered:
            raise DynamicTextureException("Attempt to bind a DynamicTexture before it has been rendered")
        Texture.bind(self, rstate)

    def getTextureRect(self, viewport):
        """Return a function that calculates the texture size taking into account
           our maximum texture size and the given root viewport size.
           """
        def fsize():
            largestPowerOfTwo = (pow(2, int(math.log(viewport.size[0]) / math.log(2))),
                                 pow(2, int(math.log(viewport.size[1]) / math.log(2))))
            return (0,0,
                    min(self.maxSize[0], largestPowerOfTwo[0]),
                    min(self.maxSize[1], largestPowerOfTwo[1]))
        return fsize

    def hasRenderState(self):
        return self.rstate is not None

    def attachRenderState(self, rstate):
        """Attach this dynamic texture to a viewport and rendering state"""
        self.rstate = copy.copy(rstate)
        rootView = rstate.viewport.rootView
        self.viewport = rootView.region(self.getTextureRect(rootView),
                                        renderLink = 'before')
        self.viewport.onDrawFrame.observe(self.drawFrame)
        self.setupViewport()
        if self.dirty:
            self.viewport.show()
        else:
            self.viewport.hide()

    def setupViewport(self):
        """A hook to let subclasses easily set up their viewport requirements.
           Defaults to ortho mode.
           """
        self.viewport.fov = None

    def integrate(self, dt):
        """Animation function called every time step. The default implementation
           only manages the render expiration timer.
           """
        self.renderLifetime += dt
        if self.expiration and self.renderLifetime > self.expiration:
            # This rendering expired, set the dirty flag and get a new expiration duration
            self.newExpiration()
            self.setDirty()

    def drawFrame(self):
        """Draw function called by our viewport"""
        self.integrate(self.time.step())
        if not self.dirty:
            self.viewport.hide()
            return

        needReconfigure = False
        for dependency in self.dependencies:
            if not dependency.rendered:
                needReconfigure = True
                self.renderDependency(dependency)
        if needReconfigure:
            self.viewport.configureOpenGL()

        self.render()

        self.dirty = False
        self.viewport.hide()
        self.rendered = True
        self.renderLifetime = 0

        ## Uncomment this to show the textures as they're stored
        #self.viewport.display.flip()
        #import time
        #time.sleep(0.5)

    def renderDependency(self, dep):
        """Force a dependency texture to render immediately"""
        if not dep.hasRenderState():
            dep.attachRenderState(self.rstate)
        dep.rstate.viewport.render()
        assert(dep.rendered)

    def render(self):
        """Render the texture. By default this calls the draw() method to draw
           the texture's contents into our viewport, then reads that viewport back.
           If this default implementation of render() is used, the subclass must
           provide a draw() function.
           """
        self.draw()
        self.loadBackbuffer(self.viewport.size, format=self.format)


class Cache:
    """Keeps track of which textures are already loaded, so we never
       duplicate texture ids
       """
    def __init__(self):
        self.textures = {}

    def load(self, name):
        if not self.textures.has_key(name):
            filename = Util.dataFile(name)

            # If the texture name has a % in it, it is a format specifier
            # for loading animated textures from a sequence of stills.
            if filename.find("%") >= 0:
                self.textures[name] = AnimatedTexture(filename)
            else:
                self.textures[name] = Texture(filename)
	return self.textures[name]


defaultCache = Cache()
def load(name):
    return defaultCache.load(name)

### The End ###

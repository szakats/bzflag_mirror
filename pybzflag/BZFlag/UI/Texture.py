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

# Keep track of the current OpenGL texture in each target, to reduce the number of binds we do
currentTexture = {}


class Texture:
    """Represents an OpenGL texture, optionally loaded from disk in any format supported by PIL"""
    def __init__(self, name=None):
        self.texture = glGenTextures(1)
        self.texEnv = GL_MODULATE
        self.target = GL_TEXTURE_2D
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
        glTexParameteri(self.target, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(self.target, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glTexParameteri(self.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)
        glTexParameteri(self.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
        if GLExtension.maxAnisotropy > 1:
            glTexParameterf(self.target, GL_TEXTURE_MAX_ANISOTROPY_EXT, GLExtension.maxAnisotropy)
        gluBuild2DMipmaps(self.target, components, w, h, format, GL_UNSIGNED_BYTE, string)

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
        global currentTexture
        if not target:
            target = self.target
        if self != currentTexture.setdefault(target, None):
            glBindTexture(target, self.texture)
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, self.texEnv)
            currentTexture[target] = self


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

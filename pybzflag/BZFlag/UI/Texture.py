""" BZFlag.UI.Texture

Texture loading utilities
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

import Image
from OpenGL.GL import *
from OpenGL.GLU import *
from BZFlag import Util, Animated


class Texture:
    """Loads a texture from the given filename into OpenGL"""
    def __init__(self, name):
        self.image = Image.open(name)
        self.w = self.image.size[0]
        self.h = self.image.size[1]
        self.image = self.image.convert('RGBA')
        self.image = self.image.tostring('raw', 'RGBA', 0, -1)

        self.texture = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D, self.texture)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
        glTexImage2D(GL_TEXTURE_2D, 0, 3, self.w, self.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, self.image)
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, self.w, self.h, GL_RGBA, GL_UNSIGNED_BYTE, self.image)

    def __del__(self):
        try:
            glDeleteTextures(self.texture)
        except:
            pass

    def bind(self):
        """Bind this texture to GL_TEXTURE_2D in the current OpenGL context"""
        glBindTexture(GL_TEXTURE_2D, self.texture)


class AnimatedTexture:
    """Load a sequence of still images for an animated texture.
       Animated textures have a printf-style format specifier to
       insert a frame number, and end with a colon and a frame rate.
       For example, radar%02d.png:15
       """
    def __init__(self, name):
        (filename, framerate) = name.split(":")
        self.frameDuration = 1 / float(framerate)
        self.frames = []
        self.frameScreenTime = 0
        self.frameNumber = 0
        self.time = Animated.Timekeeper()

        # Try to load texture frames. An error on the
        # first frame is fatal, but an error after that simply stops the loading.
        try:
            frameNumber = 0
            while True:
                self.frames.append(Texture(filename % frameNumber))
                frameNumber += 1
        except IOError:
            if not self.frames:
                raise

    def bind(self):
        self.frameScreenTime += self.time.step()
        self.frameNumber += int(self.frameScreenTime / self.frameDuration)
        self.frameScreenTime %= self.frameDuration
        self.frameNumber %= len(self.frames)
        self.frames[self.frameNumber].bind()
        

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


default_cache = Cache()
def load(name):
    return default_cache.load(name)

### The End ###

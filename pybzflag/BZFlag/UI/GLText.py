""" BZFlag.UI.GLText

Interface for drawing text in OpenGL. This uses freetype via pygame to
build texture pages holding glyph images, then it renders individual
glyphs using those texture pages.
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

import pygame, string
from pygame.locals import *
from BZFlag import Util

defaultSize = 20


class Glyph:
    """Positioning information for one glyph on a font page"""
    def __init__(self, rect, page):
        self.rect = rect
        self.page = page

    def draw(self, size):
        print self.rect


class FontPage:
    """A collection of glyphs rendered to one texture"""
    def __init__(self, font, size=(256,256)):
        self.font = font
        self.packingY = 0
        self.packingX = 0
        self.lineHeight = 0
        self.size = size
        self.surface = pygame.Surface(size)

    def allocRect(self, size):
        """Allocate space for a rectangle of the given size on the texture.
           Return a point indicating the top-left corner of the allocated
           rectangle, or None if there is no room.
           This implementation simply packs top-down left-to-right in rows.
           """
        if size[0] > self.size[0]:
            return None
        
        if self.packingX + size[0] > self.size[0]:
            # Next line
            self.packingX = 0
            self.packingY += self.lineHeight
            self.lineHeight = 0
            
        if self.packingY + size[1] > self.size[1]:
            return None
        if size[1] > self.lineHeight:
            self.lineHeight = size[1]

        allocated = (self.packingX, self.packingY)
        self.packingX += size[0]
        return allocated
        
    def pack(self, char):
        """Pack the glyph for the given character into our texture, return a Glyph.
           If there isn't enough room, return None.
           """
        # Size the glyph and allocate space for it
        glyphSize = self.font.size(char)
        allocated = self.allocRect(glyphSize)
        if not allocated:
            return None

        # Render the character and create a Glyph
        self.surface.blit(self.font.render(char, True, (255,255,255), (0,0,0)), allocated)
        return Glyph((allocated[0], allocated[1], glyphSize[0], glyphSize[1]), self)
        

class RenderedFont:
    """Represents one font size, rendered to zero or more texture 'pages'"""
    def __init__(self, filename, size):
        self.font = pygame.font.Font(filename, size)
        self.pages = [FontPage(self.font)]
        self.glyphs = {}

        # Preload the pages with common glyphs. This will load anything
        # considered a printable character in the current locale.
        for char in string.printable:
            self.getGlyph(char)

    def getGlyph(self, char):
        """Get the associated glyph for a character, either by looking up
           an existing glyph or by packing the character in an available page.
           """
        try:
            return self.glyphs[char]
        except KeyError:
            g = self.pages[-1].pack(char)
            if not g:
                self.pages.append(FontPage(self.font))
                g = self.pages[-1].pack(char)
            self.glyphs[char] = g
            return g

    def draw(self, char, size):
        self.getGlyph(char).draw(size)


class Font:
    """Represents all sizes of one scalable font."""
    def __init__(self, name, sizes=[defaultSize]):
        filename = Util.dataFile(name)
        self.sizes = {}
        self.sortedSizes = sizes
        self.sortedSizes.sort()
        for size in sizes:
            self.sizes[size] = RenderedFont(filename, size)

    def draw(self, text, size=defaultSize):
        """Draw the given text using the current OpenGL transform and color.
           All glyph escapements will cause the OpenGL modelview matrix to
           be translated.
           """
        try:
            # See if we have the exact size
            rendered = self.sizes[size]
        except KeyError:
            # Nope. Find the next larger size if we can
            rendered = None
            for renderedSize in self.sortedSizes:
                if renderedSize > size:
                    rendered = self.sizes[renderedSize]
                    break

            # If we didn't have anything big enough, just use the largest size we have
            if not rendered:
                rendered = self.sizes[self.sortedSizes[-1]]

        for char in text:
            rendered.draw(char, size)

### The End ###

""" BZFlag.UI.GLText

Interface for drawing text in OpenGL. This uses freetype via pygame to
build texture pages holding glyph images, then it renders using those
texture pages.
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
from BZFlag import Util


class RenderedFont:
    """A single font size and style, rendered to one or more textures"""
    pass



class Font:
    """Represents all sizes of one scalable font."""
    def __init__(self, filename):
        pass

    
        
def draw(position, text, color=(1,1,1,1)):
    pass



### The End ###

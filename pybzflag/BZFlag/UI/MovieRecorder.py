""" BZFlag.UI.MovieRecorder

A mostly-real-time movie recorder that saves to NuppelVideo files.
This is based on the recorder I originally wrote for Jetstream and
later ported to Crystal Space.
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

from Numeric import *


class Recorder:
    """Movie Recorder that attaches to a Viewport and records frames to
       a NuppelVideo file as they're rendered.
       """
    def __init__(self, viewport, fileTemplate="snapshot%02d.nuv"):
        self.viewport = viewport
        self.fileTemplate = fileTemplate
        self.running = False

    def start(self):
        if self.running:
            return
        self.file = open(self.getFilename(), "wb")
        self.recordHeader()

        # Insert our recorder at the beginning of the render sequence.
        # After the beginning the next frame will have started, but inserting
        # at the end isn't friendly to the way Viewport manages subviewports.
        self.viewport.renderSequence.insert(0, self.recordFrame)
        self.running = True

    def stop(self):
        if not self.running:
            return
        self.viewport.renderSequence.remove(self.recordFrame)
        self.running = False
        self.file.close()
        self.file = None

    def getFilename(self):
        return "boing"

    def recordHeader(self):
        print "squeegie"

    def recordFrame(self):
        print "poing"


class RGBtoYUV420:
    """Encapsulation for the RGB to YUV420 colorspace converter.
       This must be a class because it requires lookup tables.
       This is ported from rgb2yuv420.cpp in the jetstream/crystalspace
       movie recorder.

       YUV420 is a planar format, where the U and V planes are half the
       resolution (1/4 the number of pixels) of the Y plane. This reduces
       spacial color resolution.
       """
    def __init__(self):
        self.yr =   zeros(256, UInt32, savespace=True)
        self.yg   = zeros(256, UInt32, savespace=True)
        self.yb   = zeros(256, UInt32, savespace=True)
        self.ur   = zeros(256, UInt32, savespace=True)
        self.ug   = zeros(256, UInt32, savespace=True)
        self.vg   = zeros(256, UInt32, savespace=True)
        self.vb   = zeros(256, UInt32, savespace=True)
        self.ubvr = zeros(256, UInt32, savespace=True)

        for i in xrange(256):
            isl8 = i << 8
            yr[i]   = 65.481  * isl8
            yg[i]   = 128.553 * isl8 + 1048576
            yb[i]   = 24.966  * isl8
            ur[i]   = 37.797  * isl8
            ug[i]   = 74.203  * isl8 + 8388608
            vg[i]   = 93.786  * isl8 + 8388608
            vb[i]   = 18.214  * isl8
            ubvr[i] = 112     * isl8

    def convert(self, r, g, b):
        """Accepts three Numeric arrays for the red, green, and blue planes.
           Returns a tuple of 2-D nuemric arrays, for the Y, U, and V plane
           respectively.
           Note that for this to work right, the input width and height must
           be even numbers, and each input plane must be the same size.
           """
        (width, height) = r.shape[:2]

        # Use some fixed point math and lookup tables to calculate the luminance.
        # No resampling is performed for the Y channel.
        y = (take(self.yr, r) + take(self.yg, g) + take(self.yb, b)) >> 16

        # Calculate the U and V planes, resampling the input down by 1/2
        # Numeric's slice operator to the rescue!
        u = ( -take(self.ur, r[ ::2, ::2]) -take(self.ug, g[ ::2, ::2]) +take(self.ubvr, b[ ::2, ::2])
              -take(self.ur, r[1::2, ::2]) -take(self.ug, g[1::2, ::2]) +take(self.ubvr, b[1::2, ::2])
              -take(self.ur, r[1::2,1::2]) -take(self.ug, g[1::2,1::2]) +take(self.ubvr, b[1::2,1::2])
              -take(self.ur, r[ ::2,1::2]) -take(self.ug, g[ ::2,1::2]) +take(self.ubvr, b[ ::2,1::2]) ) >> 18

        v = (  take(self.ubvr, r[ ::2, ::2]) -take(self.vg, g[ ::2, ::2]) -take(self.vb, b[ ::2, ::2])
              +take(self.ubvr, r[1::2, ::2]) -take(self.vg, g[1::2, ::2]) -take(self.vb, b[1::2, ::2])
              +take(self.ubvr, r[1::2,1::2]) -take(self.vg, g[1::2,1::2]) -take(self.vb, b[1::2,1::2])
              +take(self.ubvr, r[ ::2,1::2]) -take(self.vg, g[ ::2,1::2]) -take(self.vb, b[ ::2,1::2]) ) >> 18

        # Cast everything back to 8 bits
        return (y.astype(UInt8), u.astype(UInt8), v.astype(UInt8))


### The End ###

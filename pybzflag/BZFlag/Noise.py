""" BZFlag.Noise

Various methods of generating noise quickly using Numeric
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

from Numeric import *
from BZFlag import Geometry


class SmoothNoise:
    """Cubically interpolated noise, in any number of dimensions.
       The parameters here are the same as described in VectorTable.
       """
    def __init__(self, dimensions, seed=None, logTableSize=None):
        self.dimensions = dimensions
        self.seed = seed
        self.table = VectorTable(dimensions, seed, logTableSize)

        self.two = array(2, Float32)
        self.three = array(3, Float32)

        # Generate the vertices of an n-dimensional cube.
        # These vertices are used as corners to sample the noise at.
        # This happens to be exactly the same as the digits of all
        # n-digit binary numbers.
        # It is important how the indices in this cube array correlate
        # binary digits to axes. See the description of the interpolation
        # algorithm below for more information.
        self.cube = zeros((1 << self.dimensions, self.dimensions))
        for point in xrange(1 << self.dimensions):
            for axis in xrange(self.dimensions):
                if point & (1<<axis):
                    self.cube[point, -(1+axis)] = 1

    def get(self, v):
        """Generate smoothed noise using the random gradient table and interpolation.
           The input can be a single vector or an array of vectors. An array of the same
           shape will be returned.
           """
        v = asarray(v).astype(Float32)
        intv = v.astype(Int)

        # Define a sampling pattern of vertices within 1 of intv in each dimension.
        # The last axis of this array will be the vertex, the next-to-last will be the
        # cube point, all other axes are as used by the caller.
        cubev = repeat(intv[...,NewAxis,:], self.cube.shape[0], -2)
        cubev += self.cube

        # Get random gradient vectors for each sample point
        grad = self.table.get(cubev)
        cubev = cubev.astype(Float32)

        # Get the distance between our vector and each sample point
        dist = repeat(v[...,NewAxis,:], self.cube.shape[0], -2) - cubev

        # Use dot products to calculate the influence of each sample point
        infl = sum(grad * dist, -1)

        # Determine the amount of interpolation in each axis.
        # This uses the curve y = 3x^2 - 2x^3 to give a much more visually
        # pleasing result. Linearly interpolated perlin noise generally looks bad.
        s = dist[...,0,:]
        s = s*s*self.three - s*s*s*self.two

        # This interpolation is easier to visulaize if you think of each axis
        # as a bit, and each sampling point as a binary number.
        # The process of interpolating over n dimensions and returning a
        # scalar result requires that we 'fold' our sample influences.
        #
        # First we fold across axis 0: This means that all samples with axis
        # zero at 0 will be matched with all samples where axis 0 is 1, and they
        # will be interpolated according to the amount in s. The result will be
        # stored in the influence array, where axis 0 is 0. The locations where
        # axis 0 is 1 will no longer be used.
        #
        # Next it folds across axis 1. Axis 0 is now 'locked', so we only have
        # half as many axes to  match up and interpolate across. The process
        # continues across all axes.
        #
        # The connection with binary digits involves how the axes are mapped
        # to locations in our cube and infl arrays. The least significant bit
        # in the cube index is the last axis, the most significant bit is the
        # first axis. Thus, reading the cube index in binary from left to right,
        # the axes are in increasing order.
        #
        # The first axis we process will then be the high bit. We fold all infl
        # samples by listing all binary numbers with this bit set to 0 and then
        # 1, and interpolate. To 'lock' this bit then is just a matter of decreasing
        # the number of axes we loop over.
        #
        # To keep track of all this bit-banging fun, we have a bit mask indicating
        # the current fold axis. The number of interpolations necessary for that fold
        # will always be equal to this mask. Every time we lock an axis, we just
        # bit-shift this mask right by one. We're done when the mask is zero.
        #
        axisNumber = 0
        axisMask = 1 << (self.dimensions - 1)
        while axisMask:
            for i in xrange(axisMask):
                infl[...,i] += s[...,axisNumber] * (infl[...,axisMask + i] - infl[...,i])
            axisMask >>= 1
            axisNumber += 1
        return infl[...,0]


class PerlinNoise(SmoothNoise):
    """Generates Perlin noise in any number of dimensions. This consists of
       several layers of noise interpolated from a deterministic random number
       generator. This is Numeric-friendly, so anywhere you request a value
       for one input coordinate you could also request a value for an
       n-dimensional array of input coordinates.

       Constructor parameters:

          seed          - The seed is combined with the input vector such that the same seed
                          and vector will produce the same output, but for any vector a different
                          seed will produce different output.
                          A seed of zero will use the current time to produce a random seed.

          octaves       - Number of layers (sized at multiples of two) to render noise at. More
                          octaves increase the detail in the noise, but also slow it down.

          persistence   - The amount each successive octave's influence on the final noise is scaled by

          logTableSize  - The base-2 logarithm of the random vector table size. A larger table will
                          produce a noise pattern with more entropy, but will also use more memory.
                          The default should be fine for most purposes.

          dimensions    - Number of dimensions the noise is generated in, and the length of the
                          expected input vectors for get()
       """
    def __init__(self, dimensions, octaves=3, persistence=0.5, seed=None, logTableSize=None):
        SmoothNoise.__init__(self, dimensions, seed, logTableSize)
        self.octaves = octaves
        self.persistence = persistence

    def get(self, v):
        """Using multiple octaves of smoothed noise, generate perlin noise"""
        v = asarray(v)
        result = zeros(v.shape[:-1], Float)
        fundamental = 1
        amplitude = 1
        for i in xrange(self.octaves):
            result += SmoothNoise.get(self, v * fundamental) * amplitude
            fundamental *= 2
            amplitude *= self.persistence
        return result


def randomArray(shape, seed=None, range=(0,1), type=Float):
    """Utility to generate a Numeric array full of pseudorandom numbers in the given range.
       This will attempt to use the RandomArray module, but fall back on using the standard
       random module in a loop.
       """
    try:
        import RandomArray
        if seed is None:
            RandomArray.seed()
        else:
            RandomArray.seed(seed+1, seed+1)
        return (RandomArray.random(shape) * (range[1]-range[0]) + range[0]).astype(type)
    except ImportError:
        import random
        random.seed(seed)
        a = zeros(multiply.reduce(shape), Float)
        for i in xrange(a.shape[0]):
            a[i] = random.random() * (range[1]-range[0]) + range[0]
        return reshape(a, shape).astype(type)


class VectorTable:
    """A table of random unit vectors generated from a seed.

       A seed of None uses the current system time.

       The size is specified as the base 2 logarithm of the actual size, since
       it must be a power of two. This is to avoid division in the get() function
       when it's necessary to constrain the input vector hash to the table size.
       """
    def __init__(self, dimensions, seed=None, logTableSize=None):
        if not logTableSize:
            logTableSize = 10

        self.size = 1 << logTableSize
        self.dimensions = dimensions

        # This generates a random array of numbers between -1 and 1, then normalizes the vectors.
        self.table = randomArray((self.size, dimensions), type=Float32, range=(-1,1), seed=seed)
        Geometry.normalize(self.table, self.table)

    def get(self, v):
        """Retrieve a random n-vector given an integer m-vector.
           An array of vectors can also be given, and an array
           of the same shape will be returned.
           """
        v = asarray(v)

        # Create a quick hash of the given vector array, to give
        # it fairly uniform coverage of our random vector array.
        hash = zeros(v.shape[:-1], v.typecode())
        m = 1
        for i in xrange(0, v.shape[-1]):
            hash += v[...,i] * m
            m += 13

        src = hash % (self.size - 1)
        if type(src) == ArrayType:
            return take(self.table, src)
        else:
            return self.table[src]

### The End ###

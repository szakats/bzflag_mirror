""" BZFlag.UI.Noise

Various methods of generating noise quickly using Numeric
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

from Numeric import *
import RandomArray
from BZFlag import Geometry


class PerlinNoise3:
    """Generates Perlin noise in 3 dimensions. This consists of several layers
       of noise interpolated from a deterministic random number generator.
       This is Numeric-friendly, so anywhere you request a value for one input
       coordinate you could also request a value for an n-dimensional array
       of input coordinates.
       
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
       """
    def __init__(self, seed=0, octaves=1, persistence=0.5, logTableSize=10):
        self.octaves = octaves
        self.persistence = persistence
        
        self.table = Vector3Table(seed, logTableSize)

        self.two = array(2, Float32)
        self.three = array(3, Float32)
        self.cube = array(((0,0,0),
                           (0,0,1),
                           (0,1,0),
                           (0,1,1),
                           (1,0,0),
                           (1,0,1),
                           (1,1,0),
                           (1,1,1)))

    def smoothNoise(self, v):
        """Generate smoothed noise using the random gradient table and interpolation.
           The input can be a single vector or an n-dimensional array of vectors. An array of
           the same shape will be returned.
           """
        v = asarray(v).astype(Float32)
        intv = v.astype(Int)
            
        # Define a sampling pattern of 8 vertices around intv
        # The last axis of this array will be the vertex, the next-to-last will be the
        # cube point, all other axes are as used by the caller.
        cubev = repeat(intv[...,NewAxis,:], 8, -2)
        cubev += self.cube

        # Get random gradient vectors for each sample point
        grad = self.table.get(cubev)
        cubev = cubev.astype(Float32)

        # Get the distance between our vector and each sample point
        dist = repeat(v[...,NewAxis,:], 8, -2) - cubev

        # Use dot products to calculate the influence of each sample point
        infl = sum(grad * dist, -1)

        # Determine the amount of interpolation in each axis.
        # This uses the curve y = 3x^2 - 2x^3 to give a much more visually
        # pleasing result. Linearly interpolated perlin noise looks really bad.
        s = dist[...,0,:]
        s = s*s*self.three - s*s*s*self.two

        # Now perform the interpolation one axis at a time
        # This has been made fairly cryptic in the interest of reducing the
        # number of copies performed- modifying an array in place is much faster
        # than performing normal infix arithmetic because a temporary array does
        # not need to be created.
        # This first combines all points along the X axis, then the Y, then Z.
        s0 = s[...,0]
        s1 = s[...,1]
        s2 = s[...,2]
        a = infl[...,0]
        b = infl[...,1]
        c = infl[...,2]
        d = infl[...,3]
        i4 = infl[...,4]
        i5 = infl[...,5]
        i6 = infl[...,6]
        i7 = infl[...,7]
        i4 -= a
        i5 -= b
        i6 -= c
        i7 -= d
        i4 *= s0
        i5 *= s0
        i6 *= s0
        i7 *= s0
        a += i4
        b += i5
        c += i6
        d += i7
        c -= a
        a += s1 * c
        d -= b
        b += s1 * d
        b -= a
        s2 *= b
        a += s2
        return a

    def get(self, v):
        """Using multiple octaves of smoothed noise, generate perlin noise"""
        result = zeros(v.shape[:-1], Float32, savespace=True)
        fundamental = 1
        amplitude = 1
        for i in xrange(self.octaves):
            result += self.smoothNoise(v * fundamental) * amplitude
            fundamental *= 2
            amplitude *= self.persistence
        return result

    
class Vector3Table:
    """A table of random 3D unit vectors generated from a seed.
       The size is specified as the base 2 logarithm of the actual size, since
       it must be a power of two. This is to avoid division in the get() function
       when it's necessary to constrain the input vector hash to the table size.
       """
    def __init__(self, seed, logTableSize):
        self.size = 1 << logTableSize

        # This generates a random array of numbers between -0.5 and 0.5, then normalizes the vectors
        RandomArray.seed(seed)
        self.table = (RandomArray.random((self.size, 3)) - 0.5).astype(Float32)
        Geometry.normalize(self.table, self.table)

    def get(self, v):
        """Retrieve a random 3-vector given an integer 3-vector.
           An n-dimensional array of vectors can also be given,
           and an array of the same shape will be returned.
           """
        v = asarray(v)
        src = (v[...,0] + v[...,1]*95 + v[...,2]*127) % (self.size - 1)
        if type(src) == ArrayType:
            return take(self.table, src)
        else:
            return self.table[src]

### The End ###

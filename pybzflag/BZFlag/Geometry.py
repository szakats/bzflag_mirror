""" BZFlag.Geometry

Geometry utilities based on the vector operations provided
by the Numeric module.
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

from Numeric import *
from LinearAlgebra import *
import math

__all__ = ['cross', 'magnitude2', 'magnitude', 'normalize', 'vectorAngle',
           'unitVectorAngle']


def cross(u, v, result=None):
    """Perform a cross product of two vectors or n-dimensional arrays of vectors"""
    u = asarray(u)
    v = asarray(v)
    if result is None:
        result = zeros(u.shape, u.typecode())
    ux = u[...,0]
    uy = u[...,1]
    uz = u[...,2]
    vx = v[...,0]
    vy = v[...,1]
    vz = v[...,2]
    
    #     [ i  j  k  ]
    # det [ ux uy uz ]
    #     [ vx vy vz ]
    result[...,0] = vz*uy - uz*vy
    result[...,1] = uz*vx - vz*ux
    result[...,2] = ux*vy - vx*uy
    return result


def magnitude2(a):
    """Calculate the squared magnitude of a vector or each vector in an n-dimensional array"""
    a = asarray(a)
    a = a*a
    result = a[...,0]
    if type(result) == ArrayType:
        add(result, a[...,1], result)
        add(result, a[...,2], result)
        return result
    else:
        # It's a single vector rather than a vector array
        return sum(a)


def magnitude(a):
    """Calculate the magnitude of a vector or each vector in an n-dimensional array"""
    a = asarray(a)
    result = magnitude2(a)
    if type(result) == ArrayType:
        sqrt(result, result)
        return result
    else:
        return sqrt(result)


def normalize(a, result=None):
    """Normalize a vector or each vector in an n-dimensional array of vectors"""
    a = asarray(a)
    try:
        if len(a.shape) > 1:
            if result is None:
                result = zeros(a.shape, a.typecode())
            divide(a, magnitude(a)[..., NewAxis], result)
            return result
        else:
            return a / magnitude(a)
    except:
        return zeros(a.shape, a.typecode())

def vectorAngle(a,b):
    """Find the angle between vectors, in degrees"""
    a = asarray(a)
    b = asarray(b)
    try:
        return arccos(dot(a,b) / (magnitude(a) * magnitude(b))) * 180 / math.pi
    except ValueError:
        # arccos of equal vectors explodes
        return 0


def unitVectorAngle(a,b):
    """Same as vectorAngle, but assumes that a and b are unit vectors"""
    a = asarray(a)
    b = asarray(b)
    try:
        return math.acos(dot(a,b)) * 180 / math.pi
    except ValueError:
        # acos of equal vectors explodes
        return 0

### The End ###

""" BZFlag.Vector

Vector math functions
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

import math


def cross(a, b):
    return (a[2] * b[1] - a[1] * b[2], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])


def length2(v):
    return v[0]*v[0] + v[1]*v[1] + v[2]*v[2]


def length(v):
    return math.sqrt(length2(v))


def normalize(v):
    l = length(v)
    try:
        return (v[0]/l, v[1]/l, v[2]/l)
    except ZeroDivisionError:
        return (0,0,0)


def sub(a,b):
    return (a[0] - b[0],
            a[1] - b[1],
            a[2] - b[2])


def add(a,b):
    return (a[0] + b[0],
            a[1] + b[1],
            a[2] + b[2])


def dot(a,b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def angle(a,b):
    """Find the angle between two vectors, in degrees"""
    try:
        return math.acos(dot(a,b) / (length(a) * length(b))) * 180 / math.pi
    except ValueError:
        # acos of equal vectors explodes
        return 0


def unitAngle(a,b):
    """Same as angle, but assumes that a and b are unit vectors"""
    try:
        return math.acos(dot(a,b)) * 180 / math.pi
    except ValueError:
        # acos of equal vectors explodes
        return 0

### The End ###

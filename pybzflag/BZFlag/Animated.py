""" BZFlag.Animated

Classes for implementing values that change over time.
This includes vectors and values that move according to
physical models or other formulas.
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

class Timekeeper:
    """A class that keeps track of the amount of time elapsed between steps"""
    def __init__(self):
        self.lastTime = None

    def time(self):
        """Return the current time"""
        import time
        return time.time()

    def step(self):
        """Take a step, return the number of seconds since the last step."""
        now = self.time()
        if self.lastTime:
            step = now - self.lastTime
        else:
            step = 0
        self.lastTime = now
        return step


class Value:
    """A value that changes over time according to a function.
       The given function should take the old value, the time step, and the vector index as parameters.
       The vector index will always be None if this value isn't used in a vector.
       This module provides several callable classes that be used in place of an actual function.
       """
    def __init__(self, value, f):
        self.value = value
        self.f = f

    def integrate(self, dt, index=None):
        self.value = self.f(self.value, dt, index)


class Vector:
    """A vector of Animated.Values.
       The given function should take the old value, the time step, and the vector index as parameters.
       This module provides several callable classes that be used in place of an actual function.
       The vector contents can be accessed transparently from this class.
       """
    def __init__(self, value, f):
        self.f = f
        self.set(value)

    def integrate(self, dt):
        for i in xrange(len(self.values)):
            self.values[i].integrate(dt, i)

    def __getitem__(self, i):
        return self.values[i].value

    def __setitem__(self, i, v):
        self.values[i].value = v

    def set(self, value):
        self.values = [Value(v, self.f) for v in value]


class Velocity:
    """A class that can be used as an integration function for Value and Vector.
       The given velocity should match the data type and number of elements.
       The velocity vector contents can be accessed transparently from this class.
       """
    def __init__(self, value):
        self.value = value
    
    def __call__(self, oldValue, dt, index=None):
        if index is None:
            return oldValue + self.value * dt
        else:
            return oldValue + self.value[index] * dt

    def __getitem__(self, i):
        return self.value[i]

    def __setitem__(self, i, v):
        self.value[i] = v


class LogApproach:
    """A class that can be used as an integration function for Value and Vector.
       Approaches a target, fast at first but exponentially slowing down to approach it.
       """
    def __init__(self, target, speed):
        self.target = target
        self.speed  = speed

    def __call__(self, oldValue, dt, index=None):
        if index is None:
            t = self.target
        else:
            t = self.target[index]
        return oldValue + (t - oldValue) * dt * self.speed

### The End ###

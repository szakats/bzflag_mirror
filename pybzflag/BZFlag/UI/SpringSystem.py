""" BZFlag.UI.SpringSystem

Spring system based simulation models and affectors
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
from BZFlag.World import Scale


class Cloth:
    """A cloth is a 2D spring system simulation. Its state is
       represented as a 3-axis Numeric array, a 2D array of
       positions for each point mass in the simulation. This can
       be used directly as a surface array for rendering the cloth.
       All cloths simulated by this class are rectangular.

       This class primarily exists to attach semantics to the
       cloth array. All actual simulation is performed by affectors
       attached to the system. By default the system has an affector
       which exerts spring forces. Extra affectors can be added for
       gravity, wind, and collisions.
       """
    def __init__(self, initialState):
        self.initialState = array(initialState)
        self.state = array(initialState)
        self.velocity = zeros(initialState.shape, initialState.typecode())
        self.affectors = [ClothSpringAffector(self),
                          VelocityAffector(self)]

    def integrate(self, dt):
        for affector in self.affectors:
            affector.integrate(dt)

    def add(self, cls, *args, **kw):
        """Add an affector to the cloth with the given class and extra
           arguments. Returns the added affector instance.
           """
        inst = cls(self, *args, **kw)
        self.affectors.append(inst)
        return inst


class Affector:
    """Abstract base class for an object that affects a simulation model"""
    def __init__(self, model):
        self.model = model

    def integrate(self, dt):
        """Affect the model for one time step"""
        pass


class ClothSpringAffector(Affector):
    """Affector that exerts spring forces on a cloth model"""
    def integrate(self, dt):
        pass


class VelocityAffector(Affector):
    """Generic affector for applying velocity to mass positions"""
    def integrate(self, dt):
        add(self.model.state, multiply(self.model.velocity, dt), self.model.state)
    

class ConstantAccelAffector(Affector):
    """Generic affector for applying a constant acceleration to masses"""
    def __init__(self, model, vector):
        Affector.__init__(self, model)
        self.vector = vector

    def integrate(self, dt):
        add(self.model.velocity, self.vector, self.model.velocity)


class GravityAffector(ConstantAccelAffector):
    """Generic affector for applying a downward gravity force"""
    def __init__(self, model):
        ConstantAccelAffector.__init__(self, model, (0,0,Scale.Gravity))
    
    
### The End ###

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
from BZFlag.Geometry import *
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
    def __init__(self, initialState, friction=0.02, stiffness=200):
        self.friction = friction
        self.stiffness = stiffness
        self.initialState = array(initialState)
        self.state = array(initialState)
        self.velocity = zeros(initialState.shape, initialState.typecode())
        self.affectors = [ClothSpringAffector(self),
                          FrictionAffector(self),
                          VelocityAffector(self)]

    def integrate(self, dt):
        # Use a fixed time step for now, since the model gets unstable
        # far too easily wiht a variable time step.
        dt = 0.02
        
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
    def integrateSprings(self, dt, span):
        """Integrate springs with the given x and y span in grid cells"""
        # Select equally shaped slices centered on each spring from the arrays
        # we're interested in. Zeroes in span[] have to be special cases here
        # since -0 does not select an entire axis.
        # This lets us process all equal-length springs at once.
        if span[0] and span[1]:
            pos_s1 = self.model.state[:-span[0], :-span[1]]
            pos_s2 = self.model.state[span[0]:, span[1]:]
            vel_s1 = self.model.velocity[:-span[0], :-span[1]]
            vel_s2 = self.model.velocity[span[0]:, span[1]:]
            init_s1 = self.model.initialState[:-span[0], :-span[1]]
            init_s2 = self.model.initialState[span[0]:, span[1]:]
        elif span[0]:
            pos_s1 = self.model.state[:-span[0], :]
            pos_s2 = self.model.state[span[0]:, :]
            vel_s1 = self.model.velocity[:-span[0], :]
            vel_s2 = self.model.velocity[span[0]:, :]
            init_s1 = self.model.initialState[:-span[0], :]
            init_s2 = self.model.initialState[span[0]:, :]
        else:
            pos_s1 = self.model.state[:, :-span[1]]
            pos_s2 = self.model.state[:, span[1]:]
            vel_s1 = self.model.velocity[:, :-span[1]]
            vel_s2 = self.model.velocity[:, span[1]:]
            init_s1 = self.model.initialState[:, :-span[1]]
            init_s2 = self.model.initialState[:, span[1]:]

        posVec = pos_s2 - pos_s1
        currentLength = magnitude(posVec)
        restLength = magnitude(init_s2 - init_s1)
        direction = normalize(posVec)
        extension = currentLength - restLength

        # This creates a 2D array of vectors, each with X, Y, and Z
        # equal to the extension from the corresponding location
        extensionVector = reshape(repeat(extension, 3, 1), direction.shape)

        # Apply a force to each mass connected to this spring,
        # proportional to the spring extension in the spring's direction
        force = dt * direction * self.model.stiffness * extensionVector
        vel_s1 += force
        vel_s2 -= force

    def integrate(self, dt):
        self.integrateSprings(dt, (1,0))
        self.integrateSprings(dt, (0,1))
        self.integrateSprings(dt, (1,1))
        self.integrateSprings(dt, (2,0))
        self.integrateSprings(dt, (2,1))
        self.integrateSprings(dt, (2,2))
        self.integrateSprings(dt, (0,2))
        self.integrateSprings(dt, (1,2))


class VelocityAffector(Affector):
    """Generic affector for applying velocity to mass positions"""
    def integrate(self, dt):
        add(self.model.state, multiply(self.model.velocity, dt), self.model.state)
    

class FrictionAffector(Affector):
    """Generic affector for applying friction to mass velocities"""
    def integrate(self, dt):
        multiply(self.model.velocity, 1 - self.model.friction, self.model.velocity)
    

class ConstantAccelAffector(Affector):
    """Generic affector for applying a constant acceleration to masses"""
    def __init__(self, model, vector):
        Affector.__init__(self, model)
        self.vector = vector

    def integrate(self, dt):
        add(self.model.velocity, self.vector, self.model.velocity)


class ClothWindAffector(Affector):
    """Affector for applying a wind force to cloth. This must be given a
       reference to the ArraySurface that renders this cloth, since it uses
       the cross products and normals that it has already calculated.
       """
    def __init__(self, model, surface, windVector):
        Affector.__init__(self, model)
        self.crossProducts = surface.crossProducts
        self.gridNormals = surface.gridNormals
        self.windVector = windVector

    def integrate(self, dt):
        forceMagnitude = dot(self.crossProducts, self.windVector)
        force = self.gridNormals * reshape(repeat(forceMagnitude, 3, 1), self.gridNormals.shape)
        self.model.velocity[:-1,:-1] += force
        
### The End ###

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

       friction      - Damping for each point mass
       stiffness     - Relative amount of force exerted by springs
       stepSize      - Size of integration steps taken
       stepTime      - Wallclock time each integration step corresponds to
       """
    def __init__(self, initialState,
                 friction      = 0.02,
                 stiffness     = 200,
                 stepSize      = 0.04,
                 stepTime      = 0.01,
                 ):
        self.stepSize = stepSize
        self.stepTime = stepTime
        self.friction = friction
        self.stiffness = stiffness

        # Time accumulator for keeping track of the temporal error
        # after taking zero or more integration steps in integrate()
        self.dt = 0

        # Initialize arrays for the simulation state
        self.initialState = array(initialState, Float32, savespace=True)
        self.state = array(initialState, Float32, savespace=True)
        self.velocity = zeros(initialState.shape, Float32, savespace=True)

        # Initial affectors to cover the spring forces,
        # friction, and velocity integration
        self.affectors = [ClothSpringAffector(self),
                          FrictionAffector(self),
                          VelocityAffector(self)]

    def reset(self):
        self.state[...] = self.initialState
        self.velocity[...] = 0

    def integrate(self, dt):
        """Integrate the spring system. Our simple method of simulation
           will quickly get unstable if our timestep is too large, so
           we convert the caller's timesteps into zero or more fixed-size
           time steps. Returns the number of integration steps performed.
           """
        self.dt += dt
        nSteps = 0
        while self.dt > 0:
            try:
                for affector in self.affectors:
                    affector.integrate(self.stepSize)

            except OverflowError:
                # If our simulation got unstable and exploded. Best we can do now is reset it.
                # This shouldn't ever happen, but this keeps us from crashing if something goes
                # terminally wrong with the simulation.
                self.reset()
                
            self.dt -= self.stepTime
            nSteps += 1            
        return nSteps

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
        # The number of springs and their orientation will determine the
        # cloth's stiffness and stability. More springs take longer to
        # simulate, of course.
        self.integrateSprings(dt, (1,0))
        self.integrateSprings(dt, (0,1))
        self.integrateSprings(dt, (1,1))
        self.integrateSprings(dt, (2,0))
        self.integrateSprings(dt, (2,2))
        self.integrateSprings(dt, (0,2))


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
    def __init__(self, model, surface, wind):
        Affector.__init__(self, model)
        self.crossProducts = surface.crossProducts
        self.gridNormals = surface.gridNormals
        self.wind = wind

    def integrate(self, dt):
        """Uses the dot product to calculate a normal force due to wind,
           applied to all masses in the spring system.
           """
        forceMagnitude = dot(self.gridNormals, tuple(self.wind))
        force = self.gridNormals * reshape(repeat(forceMagnitude, 3, 1), self.gridNormals.shape)
        self.model.velocity[:-1,:-1] += force


class ClothAnchorAffector(Affector):
    """Affector that forces the given array positions to remain at their initial values"""
    def __init__(self, model, *positions):
        Affector.__init__(self, model)
        self.positions = positions

    def integrate(self, dt):
        for x,y in self.positions:
            self.model.state[x,y] = self.model.initialState[x,y]

### The End ###

""" BZFlag.UI.ParticleSystem

Simulation models and affectors based on arrays of points. This includes
systems that can be simulated similarly, but aren't traditionally considered
particle systems, like spring and mass simulations.
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
from BZFlag.Geometry import *
from BZFlag.World import Scale


class Model:
    """An abstract particle simulation model. This defines arrays with the
       model's current state, and manages calling affectors to update the
       simulation state.
       """
    def __init__(self, initialState):
        # Initialize arrays for the simulation state
        self.initialState = array(initialState, Float32, savespace=True)
        self.state = array(initialState, Float32, savespace=True)

        # Don't add any affectors by default- let subclasses do that
        self.affectors = []

    def reset(self):
        self.state[...] = self.initialState

    def integrate(self, dt):
        """The default integration function just broadcasts the integration
           request to each affector in sequence. This catches OverflowErrors
           and sends them to integrationError().
           """
        try:
            for affector in self.affectors:
                affector.integrate(dt)
        except OverflowError:
            self.integrationError()

    def integrationError(self):
        """If our simulation got unstable and exploded. Best we can do now is reset it.
           This shouldn't ever happen, but this keeps us from crashing if something goes
           terminally wrong with the simulation.
           """
        self.reset()

    def add(self, cls, *args, **kw):
        """Add an affector to the cloth with the given class and extra
           arguments. Returns the added affector instance.
           """
        inst = cls(self, *args, **kw)
        self.affectors.append(inst)
        return inst

    def attachState(self, array):
        """Copy the model's state into the given array and perform the simulation
           directly in the array in the future. This is good to save from copying
           the simulation state when you want to send the simulation's output to a
           vertex buffer of some sort.
           """
        array[...] = self.state
        self.state = array

    def attachDrawable(self, drawable):
        """Attach this model to a drawable that supports the 'vertices' attribute.
           This may be extended in subclasses to attach other properties of the model,
           such as colors or particle sizes.
           """
        self.attachState(drawable.vertices)


class Affector:
    """Abstract base class for an object that affects a simulation model"""
    def __init__(self, model):
        self.model = model

    def integrate(self, dt):
        """Affect the model for one time step"""
        pass


class FixedIntegrationModel(Model):
    """A Model subclass which takes fixed size integration steps. This is used for models
       that can easily become unstable using variable integration steps.

       stepSize      - Size of integration steps taken
       stepTime      - Wallclock time each integration step corresponds to
       """
    def __init__(self, initialState, stepSize, stepTime):
        self.stepSize = stepSize
        self.stepTime = stepTime
        Model.__init__(self, initialState)

        # Time accumulator for keeping track of the temporal error
        # after taking zero or more integration steps in integrate()
        self.dt = 0

    def integrate(self, dt):
        """Convert the caller's timesteps into zero or more fixed-size
           time steps. Returns the number of integration steps performed.
           """
        self.dt += dt
        nSteps = 0
        while self.dt > 0:
            Model.integrate(self, self.stepSize)
            self.dt -= self.stepTime
            nSteps += 1
        return nSteps


class Cloth(FixedIntegrationModel):
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
        FixedIntegrationModel.__init__(self, initialState, stepSize, stepTime)
        self.friction = friction
        self.stiffness = stiffness

        self.velocity = zeros(initialState.shape, Float32, savespace=True)

        # Initial affectors to cover the spring forces,
        # friction, and velocity integration
        self.add(ClothSpringAffector)
        self.add(FrictionAffector)
        self.add(VelocityAffector)

    def reset(self):
        FixedIntegrationModel.reset(self)
        self.velocity[...] = 0


class Newtonian(Model):
    """A simple model for particles with position, acceleration, and velocity"""
    def __init__(self, initialState):
        Model.__init__(self, initialState)
        self.velocity = zeros(initialState.shape, Float32, savespace=True)
        self.acceleration = zeros(initialState.shape, Float32, savespace=True)
        self.add(AccelerationAffector)
        self.add(VelocityAffector)

    def reset(self):
        Model.reset(self)
        self.acceleration[...] = 0
        self.velocity[...] = 0


class Fountain(Newtonian):
    """A newtonian particle model in which each particle has a finite lifetime,
       and all new particles are generated by an emitter. Emitter is a subclass
       of Affector, and can be added with Fountain.add().
       """
    def __init__(self, numParticles):
        # All particles start off dead
        Newtonian.__init__(self, zeros((numParticles, 3)))
        self.life = zeros(numParticles, Float32, savespace=True)

    def reset(self):
        Newtonian.reset(self)
        self.life[...] = 0


class Emitter(Affector):
    """An affector that scans the model for dead particles and respawns them.
       spawnRate is the maximum particle spawning rate in particles per second.
       """
    def __init__(self, model, spawnRate, particleLifetime):
        Affector.__init__(self, model)
        self.particleLifetime = particleLifetime
        self.spawnRate = spawnRate
        self.spawnBudget = 0

    def integrate(self, dt):
        """Find all particles with life <= 0. Spawn them, limiting according to our spawn rate"""
        self.spawnBudget += self.spawnRate * dt
        deadIndices = nonzero(less_equal(self.model.life, 0))
        spawnIndices = deadIndices[:int(self.spawnBudget)]
        self.spawnBudget -= len(spawnIndices)
        self.respawn(spawnIndices)

    def respawn(self, indices):
        """Respawn particles at each model array index in the given list. By default this initializes
           position, velocity, acceleration, and life according to the emitter. More particle
           qualities for this to set can be added by subclasses.
           """
        put(self.model.state, indices, self.newState(len(indices)))
        put(self.model.velocity, indices, self.newVelocity(len(indices)))
        put(self.model.acceleration, indices, self.newAcceleration(len(indices)))
        put(self.model.life, indices, self.newLife(len(indices)))

    def newState(self, n):
        """Returns state vectors for n new particles. This may return less than n values, and they
           will be repeated as necessary. In particular, it can return a scalar. The other newFoo
           functions here obey the same semantics.
           """
        return (0,0,0)

    def newVelocity(self, n):
        return (0,0,0)

    def newAcceleration(self, n):
        return (0,0,0)

    def newLife(self, n):
        return self.particleLifetime


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
    """Generic affector for applying velocity to particle positions"""
    def integrate(self, dt):
        add(self.model.state, multiply(self.model.velocity, dt), self.model.state)


class AccelerationAffector(Affector):
    """Generic affector for applying acceleration to particle velocities"""
    def integrate(self, dt):
        add(self.model.velocity, multiply(self.model.acceleration, dt), self.model.velocity)


class FrictionAffector(Affector):
    """Generic affector for applying friction to particle velocities"""
    def integrate(self, dt):
        multiply(self.model.velocity, 1 - self.model.friction, self.model.velocity)


class ConstantAccelAffector(Affector):
    """Generic affector for applying a constant acceleration to particles"""
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
           applied to all particles in the spring system.
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

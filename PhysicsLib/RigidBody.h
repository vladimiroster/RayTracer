#pragma once

#include <memory>

#include "Collidable.h"

#include "../RayTracerLib/Object.h"
#include "../RayTracerLib/World.h"

namespace rt = RayTracer;

class rt::World;

namespace Physics {

  class RigidBody : public Collidable {
  public:
    RigidBody(rt::World& w, rt::Object* obj, float mass, bool obeys_gravity = true, rt::Vector velocity = rt::zero_vec, rt::Vector acceleration = rt::zero_vec, rt::Vector aVelocity = rt::zero_vec, rt::Vector aAcceleration = rt::zero_vec) 
      : Collidable(w, obj, mass, velocity, acceleration, aVelocity, aAcceleration), _obeys_gravity(obeys_gravity) {}

    virtual void setup() override {}

    virtual void action() override;

    virtual void apply_force(rt::Vector force) override {
      force = force / _mass;
      // TODO: implement += for vectors
      _acceleration = _acceleration + force;

      // TODO: model bouncing with a spring force
    }

    bool obeys_gravity() const {
      return _obeys_gravity;
    }

    virtual void collide(Collidable& other) override {
      apply_force(other.acceleration() * other.mass());
      other.apply_force((other.acceleration() + other.velocity()) * (-1) * other.mass());
    }

    void apply_friction(float mu, float N) {
      rt::Vector friction = rt::normalize(_velocity) * (-1) * mu * N;
      std::cout << "Applying friction: " << friction << "(" << _velocity << ", " << rt::normalize(_velocity) << ")" << std::endl;
      apply_force(friction);
    }

    void apply_drag(float c) {
      // TODO: drag with surface area
      // TODO: lift induced drag (wing rise)
      auto speed = rt::magnitude(_velocity);
      auto force_mag = std::min(c * speed * speed, speed);
      apply_force(rt::normalize(_velocity) * force_mag * (-1));
    }

  private:
    bool _obeys_gravity;
  };

} // namespace Physics

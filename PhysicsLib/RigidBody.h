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
    RigidBody(rt::World& w, rt::Object* obj, float mass, rt::Vector velocity = rt::zero_vec, rt::Vector acceleration = rt::zero_vec, rt::Vector aVelocity = rt::zero_vec, rt::Vector aAcceleration = rt::zero_vec, bool obeys_gravity = true) 
      : Collidable(w, obj, mass, velocity, acceleration, aVelocity, aAcceleration), _obeys_gravity(obeys_gravity) {}

    virtual void setup() override {}

    virtual bool action() override;

    virtual void apply_force(rt::Vector force) override {
      force = force / _mass;
      // TODO: implement += for vectors
      _acceleration = _acceleration + force;

      // TODO: model bouncing with a spring force (chapter 3)
    }

    void apply_local_force(rt::Vector force) {
      rt::Vector local_force = _obj->transform() * force;
      apply_force(local_force);
    }

    void apply_angular_force(rt::Vector aForce) {
      aForce = aForce / _mass;
      _aAcceleration = _aAcceleration + aForce;
    }

    void apply_local_angular_force(rt::Vector aForce) {
     // aForce = _obj->inverse() * aForce;
      apply_angular_force(aForce);
    }

    bool obeys_gravity() const {
      return _obeys_gravity;
    }

    virtual void collide(Collidable& other) override {
      apply_force(other.acceleration() * other.mass());
      other.apply_force((other.acceleration() + other.velocity()) * (-1) * other.mass());
    }

    void apply_friction(float mu, float N) {
      // TODO: friction on an incline
      rt::Vector friction = rt::normalize(_velocity) * (-1) * mu * N;
      apply_force(friction);
    }

    void apply_drag(float c) {
      // TODO: drag with surface area
      // TODO: lift induced drag (wing rise)
      // Linear drag
      auto speed = rt::magnitude(_velocity);
      auto force_mag = std::min(c * speed * speed, speed);
      apply_force(rt::normalize(_velocity) * force_mag * (-1));
      // Angular drag
      speed = rt::magnitude(_aVelocity);
      force_mag = std::min(c * speed * speed, speed);
      apply_angular_force(rt::normalize(_aVelocity) * force_mag * (-1));
    }

  private:
    bool _obeys_gravity;
  };

} // namespace Physics

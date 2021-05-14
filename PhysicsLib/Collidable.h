#pragma once

#include "Behavior.h"
#include "RigidBody.h"

#include "../RayTracerLib/Object.h"
#include "../RayTracerLib/World.h"

namespace rt = RayTracer;

class rt::World;

namespace Physics {

  class Collidable : public Behavior {
  public:
    Collidable(rt::World& w, rt::Object* obj, float mass, rt::Vector velocity = rt::zero_vec, rt::Vector acceleration = rt::zero_vec, rt::Vector aVelocity = rt::zero_vec, rt::Vector aAcceleration = rt::zero_vec) 
      : _world(w), _obj(obj), _mass(mass), _velocity(velocity), _acceleration(acceleration), _aVelocity(aVelocity), _aAcceleration(aAcceleration) {}

    rt::Point location() const {
      auto transform = _obj->transform();

      return rt::Point(transform[0][3], transform[1][3], transform[2][3]);
    }

    float mass() const {
      return _mass;
    }

    rt::Vector velocity() const {
      return _velocity;
    }

    rt::Vector acceleration() const {
      return _acceleration;
    }

    virtual void collide(Collidable& other) = 0;

    virtual void apply_force(rt::Vector force) = 0;

  protected:
    // TODO: world should not be here (or in RB for that matter)
    rt::World& _world;
    rt::Object* _obj;
    float _mass;
    rt::Vector _velocity;
    rt::Vector _acceleration;
    rt::Vector _aVelocity;
    rt::Vector _aAcceleration;
  };

} // namespace Physics

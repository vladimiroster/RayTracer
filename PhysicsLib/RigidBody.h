#pragma once

#include "Behavior.h"

#include "../RayTracerLib/Object.h"

namespace rt = RayTracer;

namespace Physics {

  class RigidBody : public Behavior {
  public:
    RigidBody(rt::Object* obj, rt::Vector velocity = rt::zero_vec, rt::Vector acceleration = rt::zero_vec) 
      : _obj(obj), _velocity(velocity), _acceleration(acceleration) {}

    virtual void setup() override {}

    virtual void action() override {
      if (_obj) {
        auto transform = _obj->transform();

        _velocity = _velocity + _acceleration;
        rt::Point loc(transform[0][3], transform[1][3], transform[2][3]);
        _obj->move(transform * rt::Transform::id().translate(_velocity));
        _acceleration = rt::zero_vec;
      }
    }

    void apply_force(rt::Vector force) {
      // TODO: implement += for vectors
      _acceleration = _acceleration + force;
    }

  private:
    rt::Object* _obj;
    rt::Vector _velocity;
    rt::Vector _acceleration;
  };

} // namespace Physics

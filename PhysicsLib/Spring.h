#pragma once

#include "Behavior.h"
#include "RigidBody.h"

#include "../RayTracerLib/Object.h"

namespace rt = RayTracer;

namespace Physics {

  class Spring : public Behavior {
  public:
    Spring(rt::Object* obj, rt::Object* other, float k) 
      : _obj(obj), _loc(obj->transform()[0][3], obj->transform()[1][3], obj->transform()[2][3]), 
      _other(other->transform()[0][3], other->transform()[1][3], other->transform()[2][3]), _k(k) {}

    virtual void setup() override {}

    virtual bool action() override {

      // TODO: create callbacks in rigidbody to have objects sign up to apply force
      if (_obj) {
        rt::Vector dirVec = rt::normalize(_other - _loc);
        for (std::shared_ptr<Behavior> b : _obj->behaviors()) {
          auto rb = std::dynamic_pointer_cast<RigidBody>(b);
          if (!rb) {
            continue;
          }
          // Apply force towards the spring
          auto cosAngle = rt::dot(rb->force(), dirVec) / rt::magnitude(rb->force());
          rt::Vector force = rb->force() * cosAngle;
          std::cout << "force: " << rb->force() << std::endl;
          std::cout << "spring: " << force << std::endl;
          rb->apply_force(force * rb->mass());
        }

      }

      return true;
    }

  protected:
    rt::Object* _obj;
    // TODO: make a method for extracting loc
    rt::Point _loc;
    rt::Point _other;
    float _k;
  };

} // namespace Physics
#pragma once

#include <memory>

#include "RigidBody.h"

#include "../RayTracerLib/Object.h"

namespace rt = RayTracer;

namespace Physics {

  class Engine {
  public:
    Engine(rt::Vector gravity = rt::zero_vec) : _g(gravity) {}

    void act(std::vector<std::shared_ptr<rt::Object>> objects) {
      for (auto obj : objects) {
        for (auto b : obj->behaviors()) {
          auto rb = std::dynamic_pointer_cast<RigidBody>(b);
          if (!rb) {
            continue;
          }
          // Apply gravity
          //rb->apply_force(_g * rb->mass());

          // Apply wind
          //rb->apply_force(rt::Vector(0.05f, 0, 0));
          
          // Friction
          rb->apply_friction(0.1f, 1);
        }
      }
    }

  private:
    rt::Vector _g;
  };

} // namespace Physics

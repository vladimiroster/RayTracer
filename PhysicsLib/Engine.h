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
        auto rb = std::dynamic_pointer_cast<RigidBody>(obj->behavior());
        if (!rb) {
          continue;
        }
        // Apply gravity
        rb->apply_force(_g);
      }
    }

  private:
    rt::Vector _g;
  };

} // namespace Physics

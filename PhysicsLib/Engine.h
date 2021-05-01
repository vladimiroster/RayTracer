#pragma once

#include <memory>

#include "RigidBody.h"

#include "../RayTracerLib/Object.h"
#include "../RayTracerLib/World.h"

namespace rt = RayTracer;

namespace Physics {

  class Engine {
  public:
    Engine(rt::World& world, rt::Vector gravity = rt::zero_vec) : _world(world), _g(gravity) {}

    void act() {
      for (auto obj : _world.objects()) {
        for (auto b : obj->behaviors()) {
          auto rb = std::dynamic_pointer_cast<RigidBody>(b);
          if (!rb) {
            continue;
          }

          // Apply gravity
          if (rb->obeys_gravity()) {
            rb->apply_force(_g * rb->mass());
          }

          // Apply wind
          //rb->apply_force(rt::Vector(0.05f, 0, 0));
          
          // Friction
          //rb->apply_friction(0.2f, 1);
        }
      }
    }

  private:
    rt::Vector _g;
    rt::World& _world;
  };

} // namespace Physics

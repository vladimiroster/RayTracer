#pragma once

#include <memory>

#include "RigidBody.h"

#include "../RayTracerLib/Object.h"
#include "../RayTracerLib/World.h"

namespace rt = RayTracer;

namespace Physics {

  class Engine {
  public:
    Engine(rt::World& world, float gravity = 1) : _world(world), _g(gravity) {}

    void act() {
      for (auto obj : _world.objects()) {
        for (auto b : obj->behaviors()) {
          auto rb = std::dynamic_pointer_cast<RigidBody>(b);
          if (!rb) {
            continue;
          }

          for (auto obj2 : _world.objects()) {
            if (obj2 == obj) {
              continue;
            }
            for (auto b2 : obj2->behaviors()) {
              auto rb2 = std::dynamic_pointer_cast<RigidBody>(b2);
              if (!rb2) {
                continue;
              }

              // Apply gravity
              if (rb->obeys_gravity() && rb2->obeys_gravity()) {
                rt::Vector force = rb->location() - rb2->location();
                auto distance = rt::magnitude(force);
                float m = (_g * rb->mass() * rb2->mass()) / (distance * distance + rt::epsilon);
                force = rt::normalize(force) * m;
                rb->apply_force(force * -1.0f);
                rb2->apply_force(force);
              }
            }
          }

          // Apply wind
          //rb->apply_force(rt::Vector(0.05f, 0, 0));
          
          // Friction
          //rb->apply_friction(0.2f, 1);

          // Air drag
          rb->apply_drag(0.8f);
        }
      }
    }

  private:
    //rt::Vector _g;
    float _g;
    rt::World& _world;
  };

} // namespace Physics

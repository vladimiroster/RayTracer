#pragma once

#include "Behavior.h"
#include "Collidable.h"

#include "../RayTracerLib/Object.h"
#include "../RayTracerLib/World.h"

namespace rt = RayTracer;

class rt::World;

namespace Physics {

  class Liquid : public Collidable {
  public:
    Liquid(rt::World& w, rt::Object* obj, float c) 
      : Collidable(w, obj, 0), _c(c) {}

    virtual void setup() override {}

    virtual void action() override {
      // Apply drag on all RBs inside
      for (auto floater : _floaters) {
        floater->apply_drag(_c);
      }
    }

    rt::Point location() const {
      auto transform = _obj->transform();

      return rt::Point(transform[0][3], transform[1][3], transform[2][3]);
    }

    virtual void collide(Collidable& other) override {
      // TODO: better way to do this?
      RigidBody *rb = dynamic_cast<RigidBody*>(&other);
      if (rb) {
        auto it = std::find(_floaters.begin(), _floaters.end(), rb);
        if (it == _floaters.end()) {
          _floaters.push_back(rb);
        } else {
          _floaters.erase(it);
        }
      }
    }

    virtual void apply_force(rt::Vector f) override {}

  private:
    float _c;

    std::vector<RigidBody*> _floaters;
  };

} // namespace Physics

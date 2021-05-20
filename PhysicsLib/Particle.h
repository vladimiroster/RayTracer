#pragma once

#include <memory>

#include "Collidable.h"

#include "../RayTracerLib/Object.h"
#include "../RayTracerLib/World.h"
#include "../RayTracerLib/Cube.h"

namespace rt = RayTracer;

class rt::World;

namespace Physics {

  class Particle : public RigidBody {
  public:
    Particle(rt::World& w, rt::Object* obj, float mass, rt::Vector velocity = rt::zero_vec, rt::Vector acceleration = rt::zero_vec, rt::Vector aVelocity = rt::zero_vec, rt::Vector aAcceleration = rt::zero_vec, bool obeys_gravity = true, size_t lifespan = SIZE_MAX) 
      : RigidBody(w, obj, mass, velocity, acceleration, aVelocity, aAcceleration, obeys_gravity), _lifespan(lifespan) {}

    virtual bool action() override {
      RigidBody::action();
      _lifespan -= 1;

      return isAlive();
    }

    virtual void collide(Collidable& other) override {
      // TODO: no collisions here yet
    }

    bool isAlive() {
      return _lifespan > 0;
    }

  private:
    size_t _lifespan;
  };

  // TODO: maybe this doesn't need to be behavior at all
  class ParticleSystem : public Behavior {
  public:
    ParticleSystem(rt::World& w, size_t num) : _w(w), _num(num) { // TODO: pass material, object type, etc
    }

    virtual void setup() override {
      emit(rt::zero_vec, rt::zero_vec);
    }

    virtual bool action() override {
      return true;
    }

    void emit(rt::Vector origin, rt::Vector direction) {
      rt::Material cube_mat(rt::Color(0.1f, 0.8f, 1), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
      for (auto i = 0; i < _num; ++i) {
        _w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.33f, 0.33f, 0.33f).translate(origin), cube_mat));
        _w.objects().back()->behaviors().emplace_back(std::make_shared<Physics::Particle>(_w, _w.objects().back().get(), 2.0f, direction, rt::zero_vec, rt::zero_vec, rt::zero_vec, true, i + 8));
      }
    }

  private:
    size_t _num;
    rt::World& _w;
  };

} // namespace Physics

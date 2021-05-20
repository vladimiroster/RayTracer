#pragma once
#include <GLFW/glfw3.h>

#include "../PhysicsLib/InputSystem.h"
#include "../PhysicsLib/Particle.h"

#include "../RayTracerLib/Sphere.h"

class AsteroidsInputSystem : public Physics::LinearInputSystem {
public:
  AsteroidsInputSystem(rt::Object* obj, const std::map<int, rt::Vector>& keyMap, rt::World& w) :
    LinearInputSystem(obj, keyMap), _system(w, 3, rt::Vector(1, 1, 0), rt::Color(0.1f, 0.8f, 1)) {};

protected:
  virtual void local_action() override {
    LinearInputSystem::local_action();
    auto accel = _rb->acceleration();
    if (rt::magnitude(accel) > 0) {
      rt::Vector dir = rt::normalize(accel);
      if (_test) {
        _system.emit(_rb->location(), dir * -1);
        //_test = false;
      }
    }
  }

  Physics::ParticleSystem _system;
  bool _test = true;
};

class Target : public Physics::Collidable {
public:
  Target(rt::World& w, rt::Object* obj, float mass, rt::Vector velocity = rt::zero_vec, rt::Vector acceleration = rt::zero_vec, rt::Vector aVelocity = rt::zero_vec, rt::Vector aAcceleration = rt::zero_vec) 
    : Collidable(w, obj, mass, velocity, acceleration, aVelocity, aAcceleration) {}

  virtual void setup() override {}

  virtual bool action() override {
    return _alive;
  }

  virtual void collide(Collidable& other) override {
    if (_alive) {
      Physics::ParticleSystem system(_world, 30, rt::Vector(2, 2, 0), rt::Color(1, 0, 0));

      system.emit(location(), rt::Vector(0, 0, 0));
    }
    _alive = false;
  }

  virtual void apply_force(rt::Vector force) override {}

private:
  bool _alive = true;
};

void asteroids(rt::World& w);
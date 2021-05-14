#pragma once

#include "Behavior.h"

#include "../RayTracerLib/Object.h"

namespace rt = RayTracer;

namespace Physics {

  // General moves-in-time object
  class Mover : public Behavior {
  public:
    Mover(rt::Object* obj) : _obj(obj), _t(0) {}

    virtual void setup() override {}

    virtual void action() override {
      if (_obj) {
        move();
        ++_t;
      }
    }

    virtual void move() = 0;

  protected:
    rt::Object* _obj;
    size_t _t;
  };

  class HarmonicMover : public Mover {
  public:
    HarmonicMover(rt::Object* obj, float amplitude, float period, rt::Vector dir) :
      Mover(obj), _amplitude(amplitude), _period(period), _direction(dir), _initialTransform(_obj->transform()) {}

    virtual void move() override {
      auto y = _amplitude * sinf(3.1415f * 2 * _t / _period * 2);
      auto x = _amplitude * 2 * sinf(3.1415f * 2 * _t / _period);
      _obj->move(_initialTransform * rt::Transform::id().translate(x, y, 0));
    }

  protected:
    float _amplitude;
    float _period;
    rt::Vector _direction;

    rt::Transform _initialTransform;
  };

  class AngularHarmonicMover : public HarmonicMover {
  public:
    AngularHarmonicMover(rt::Object* obj, float amplitude, float period, rt::Vector dir) :
      HarmonicMover(obj, amplitude, period, dir) {}

    virtual void move() override {
      auto x = _amplitude * sin(3.1415f * 2 * _t / _period);
      _obj->move(_initialTransform * rt::Transform::id().rot_y(x));
    }
  };
} // namespace Physics
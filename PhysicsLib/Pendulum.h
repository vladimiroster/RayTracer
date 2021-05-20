#pragma once

#include "Behavior.h"

#include "../RayTracerLib/Object.h"

namespace rt = RayTracer;

namespace Physics {

  class Pendulum : public Behavior {
  public:
    Pendulum(rt::Object* obj, float angle, rt::Point origin, float r, float g) 
      : _obj(obj), _angle(angle), _origin(origin), _r(r), _g(g) {}

    virtual void setup() override {}

    virtual bool action() override {
      if (_obj) {
        _aAcceleration = sin(_angle) * -_g / _r;
        _aVelocity = _aVelocity + _aAcceleration;
        _angle = _angle + _aVelocity;

        _aVelocity = _aVelocity * 0.99f;

        rt::Vector location(_r, 3.1415f / 2 + _angle, -1);
        rt::Vector loc = _origin + rt::carthesian(location);
        _obj->move(rt::Transform::id().translate(loc));
      }
      return true;
    }

  protected:
    rt::Object* _obj;
    float _angle;
    rt::Point _origin;
    float _r;
    float _g;

    float _aVelocity = 0;
    float _aAcceleration = 0;
  };

} // namespace Physics
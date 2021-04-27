#pragma once

#include "Behavior.h"
#include "Distribution.h"

#include "../RayTracerLib/Object.h"

namespace rt = RayTracer;

namespace Physics {

  class BouncingObject : public Behavior {
  public:
    BouncingObject(rt::Object* obj, rt::Vector corner1, rt::Vector corner2, rt::Vector speed) :
      _obj(obj), _c1(corner1), _c2(corner2), _speed(speed) {}

    virtual void setup() override {}

    virtual void action() override {
      if (_obj) {
        auto transform = _obj->transform();
        // TODO: make that intersection with objects - receive a bunch of objects, cast rays to all of them, and reflect if xs time is < delta
        rt::Point loc(transform[0][3], transform[1][3], transform[2][3]);

        if ((loc.x > _c2.x) || (loc.x < _c1.x)) {
          _speed.x *= -1;
        }
        if ((loc.y > _c2.y) || (loc.y < _c1.y)) {
          _speed.y *= -1;
        }
        if ((loc.z > _c2.z) || (loc.z < _c1.z)) {
          _speed.z *= -1;
        }
        rt::Vector v = _speed + rt::Vector(1, 1, 1) * rt::magnitude(_speed) * 0.5f * _noise.noise(loc.x, loc.y, loc.z);
        _obj->move(transform * rt::Transform::id().translate(v));
      }
    }

  private:
    rt::Object* _obj;
    rt::Vector _c1;
    rt::Vector _c2;
    rt::Vector _speed;
    PerlinNoise _noise;
  };

} // namespace Physics
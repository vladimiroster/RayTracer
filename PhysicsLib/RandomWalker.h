#pragma once

#include <random>

#include "Behavior.h"

#include "../RayTracerLib/Object.h"

namespace rt = RayTracer;

namespace Physics {

  template <class DistrX, class DistrY = DistrX, class DistrZ = DistrY>
  class RandomWalker : public Behavior {
  public:
    RandomWalker(rt::Object* obj, DistrX disx, DistrY disy, DistrZ disz) : _obj(obj),
      _disx(disx), _disy(disy), _disz(disz) {}

    virtual void setup() override {}
    virtual void action() override {
      if (_obj) {
        auto transform = _obj->transform();
        _obj->move(transform * rt::Transform::id().translate(_disx(), _disy(), _disz()));
      }
    }

  private:
    rt::Object* _obj;
    DistrX _disx;
    DistrY _disy;
    DistrZ _disz;
  };

} // namespace Physics
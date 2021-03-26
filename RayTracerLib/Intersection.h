#pragma once

#include <vector>
#include <optional>
#include <functional>

#include "Tuple.h"
#include "Ray.h"

namespace RayTracer {
  class Object;

  class Intersection {
  private:
    float _t;
    // TODO: Handle scope problem. Maybe use weak pointer?
    std::reference_wrapper<const Object> _o;

  public:
    Intersection(float time, const Object& obj) : _t(time), _o(obj) {}

    float time() const {
      return _t;
    }

    const Object& object() const {
      return _o.get();
    }

    struct Computation {
      float time;
      std::reference_wrapper<const Object> object;
      Point point;
      Point over_point;
      Point under_point;
      Vector eyev;
      Vector normalv;
      Vector reflectv;
      bool inside;
      float n1;
      float n2;

      Computation(float t, const Object& obj, Point p, Vector eye, Vector normal, Vector reflect, float refl_n1, float refl_n2);
    };

    Computation precompute(const Ray& r, const std::vector<Intersection>& xs) const;

    bool operator==(const Intersection& rhs) const;
    bool operator!=(const Intersection& rhs) const;
  };


  std::optional<Intersection> hit(const std::vector<Intersection>& xs);

  float schlick(const Intersection::Computation& comps);

} // namespace RayTracer
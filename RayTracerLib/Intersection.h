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

    // TODO: this is a pretty large epsilon, figure out why needed
    static inline constexpr float SHADE_EPS = 0.00001f;

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
      Vector eyev;
      Vector normalv;
      bool inside;

      Computation(float t, const Object& obj, Point p, Vector eye, Vector normal);
    };

    Computation precompute(const Ray& r) const;

    bool operator==(const Intersection& rhs) const;
    bool operator!=(const Intersection& rhs) const;
  };


  std::optional<Intersection> hit(const std::vector<Intersection>& xs);

} // namespace RayTracer
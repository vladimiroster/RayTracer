#pragma once

#include <vector>
#include <optional>
#include <functional>

namespace RayTracer {
  struct Sphere;

  struct Intersection {
    float t;
    // TODO: Handle scope problem. Maybe use weak pointer?
    std::reference_wrapper<const Sphere> s;

    Intersection(float time, const Sphere& sph) : t(time), s(sph) {}
  };

  bool operator==(const Intersection& lhs, const Intersection& rhs);
  bool operator!=(const Intersection& lhs, const Intersection& rhs);

  std::optional<Intersection> hit(const std::vector<Intersection>& xs);

} // namespace RayTracer
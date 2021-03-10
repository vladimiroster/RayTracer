#include "Plane.h"

namespace RayTracer {
  std::vector<Intersection> Plane::intersect_local(const Ray & r) const
  {
    if (abs(r.direction().y) < epsilon) {
      return {};
    }

    // Works because plane is in xz
    auto t = -r.origin().y / r.direction().y;
    return std::vector<Intersection>{Intersection(t, *this)};
  }
} // namespace RayTracer
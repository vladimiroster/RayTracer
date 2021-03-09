#include "Intersection.h"

#include <optional>

#include "Tuple.h"
#include "Object.h"

namespace RayTracer {

  bool Intersection::operator==(const Intersection & rhs) const
  {
    if (this == &rhs) {
      return true;
    }

    return ApproxEqual(_t, rhs._t, epsilon) && &_o.get() == &rhs._o.get();
  }

  bool Intersection::operator!=(const Intersection & rhs) const
  {
    return !(*this == rhs);
  }

  Intersection::Computation Intersection::precompute(const Ray& r) const {
    auto p = r.position(_t);
    return Computation(_t, _o, p, -r.direction(), _o.get().normal(p));
  }

  std::optional<Intersection> hit(const std::vector<Intersection>& xs)
  {
    // TODO: "intersections" can be an object that calculates the heap when new intersections are added.
    // Reevaluate when the client code for that will be written
    // Even better - have two heaps, one for positive and one for negative distances for easy access to both
    std::vector<Intersection> non_neg;
    non_neg.reserve(xs.size());
    std::copy_if(xs.begin(), xs.end(), std::back_inserter(non_neg), [](const Intersection& i) {
      return i.time() >= 0;
      });
    std::make_heap(non_neg.begin(), non_neg.end(), [](const Intersection& i1, const Intersection& i2) {
      return i1.time() > i2.time(); });
    if (non_neg.size() > 0) {
      return std::optional<Intersection>(std::in_place, non_neg.front().time(), non_neg.front().object());
    }
    else {
      return {};
    }
  }

  Intersection::Computation::Computation(float t, const Object & obj, Point p, Vector eye, Vector normal) :
  time(t), object(obj), point(p), eyev(eye), normalv(normal) 
  {
    if (dot(normalv, eyev) < 0) {
      inside = true;
      normalv = -normalv;
    }
    else {
      inside = false;
    }
  }

} // namespace RayTracer
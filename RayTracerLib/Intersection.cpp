#include "Intersection.h"

#include <optional>

#include "Tuple.h"
#include "Sphere.h"

bool RayTracer::Intersection::operator==(const Intersection & rhs) const
{
  if (this == &rhs) {
    return true;
  }

  return ApproxEqual(_t, rhs._t, epsilon) && &_s.get() == &rhs._s.get();
}

bool RayTracer::Intersection::operator!=(const Intersection & rhs) const
{
  return !(*this == rhs);
}

std::optional<RayTracer::Intersection> RayTracer::hit(const std::vector<Intersection>& xs)
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
    return std::optional<Intersection>(std::in_place, non_neg.front().time(), non_neg.front().sphere());
  }
  else {
    return {};
  }
}

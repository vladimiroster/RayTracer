#include "Intersection.h"

#include <optional>

#include "Tuple.h"
#include "Sphere.h"

bool RayTracer::operator==(const Intersection & lhs, const Intersection & rhs)
{
  return ApproxEqual(lhs.t, rhs.t, epsilon) && &lhs.s.get() == &rhs.s.get();
}

bool RayTracer::operator!=(const Intersection & lhs, const Intersection & rhs)
{
  return !(lhs == rhs);
}

std::optional<RayTracer::Intersection> RayTracer::hit(const std::vector<Intersection>& xs)
{
  // TODO: "intersections" can be an object that calculates the heap when new intersections are added.
  // Reevaluate when the client code for that will be written
  // Even better - have two heaps, one for positive and one for negative distances for easy access to both
  std::vector<Intersection> non_neg;
  non_neg.reserve(xs.size());
  std::copy_if(xs.begin(), xs.end(), std::back_inserter(non_neg), [](const Intersection& i) {
    return i.t >= 0;
    });
  std::make_heap(non_neg.begin(), non_neg.end(), [](const Intersection& i1, const Intersection& i2) {
    return i1.t > i2.t; });
  if (non_neg.size() > 0) {
    return std::optional<Intersection>(std::in_place, non_neg.front().t, non_neg.front().s);
  }
  else {
    return {};
  }
}

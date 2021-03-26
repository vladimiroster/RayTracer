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

  Intersection::Computation Intersection::precompute(const Ray& r, const std::vector<Intersection>& xs) const {
    // TODO: brute force implementation, looks like a lot of potential for optimization here
    std::vector<std::reference_wrapper<const Object>> containers;
    float n1 = 1, n2 = 1;
    for (auto i : xs) {
      // If this is the hit
      if (i == *this) {
        if (containers.size() > 0) {
          n1 = containers.back().get().material().refractive_index;
        }
      }

      // TODO: better way to find reference_wrapper?
      auto outer = std::find_if(containers.begin(), containers.end(), [i](std::reference_wrapper<const Object> r) { return r.get() == i.object();});
      if (outer == containers.end()) {
        // Not there means we're entering that object
        containers.push_back(std::ref(i.object()));
      }
      else {
        // Exiting that object
        containers.erase(outer);
      }
      if (i == *this) {
        if (containers.size() > 0) {
          n2 = containers.back().get().material().refractive_index;
          break;
        }
      }
    }

    auto p = r.position(_t);
    auto normal = _o.get().normal(p);
    auto reflectvec = reflect(r.direction(), normal);
    return Computation(_t, _o, p, -r.direction(), normal, reflectvec, n1, n2);
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

  Intersection::Computation::Computation(float t, const Object & obj, Point p, Vector eye, Vector normal, Vector reflect, float refl_n1, float refl_n2) :
  time(t), object(obj), point(p), over_point(p), under_point(p), eyev(eye), normalv(normal), reflectv(reflect), n1(refl_n1), n2(refl_n2)
  {
    if (dot(normalv, eyev) < 0) {
      inside = true;
      normalv = -normalv;
    }
    else {
      inside = false;
    }
    // Need to first (possibly) negate the normal
    over_point = point + normalv * epsilon;
    under_point = point - normalv * epsilon;
  }

  float schlick(const Intersection::Computation& comps) {
    auto cos = dot(comps.eyev, comps.normalv);

    // Total internal reflection can occur only if n1 > n2
    if (comps.n1 > comps.n2) {
      auto n = comps.n1 / comps.n2;
      auto sin2_t = n * n * (1.0f - cos * cos);
      if (sin2_t > 1.0f) {
        return 1.0f;
      }

      auto cos_t = sqrt(1.0f - sin2_t);

      // When n1 > n2 use cos(thetha_t) instead
      cos = cos_t;
    }

    auto calc = ((comps.n1 - comps.n2) / (comps.n1 + comps.n2));
    auto r0 = calc * calc;
    return r0 + (1 - r0) * powf((1 - cos), 5);
  }
} // namespace RayTracer
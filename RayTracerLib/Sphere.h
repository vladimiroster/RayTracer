#pragma once

#include <vector>

#include "Tuple.h"
#include "Ray.h"
#include "Intersection.h"
#include "Transformation.h"

namespace RayTracer {

  struct Sphere {
    // TODO: class invariant starts being non-trivial, switch to encapsulated mode soon
    Transform transform;
    Transform inverse;

    Sphere(Transform t = Transform::id()) : transform(t), inverse(t.inverse()) {}

    std::vector<Intersection> intersect(Ray r);
  };

} // namespace RayTracer
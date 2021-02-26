#pragma once

#include <vector>

#include "Tuple.h"
#include "Ray.h"
#include "Intersection.h"
#include "Transformation.h"

namespace RayTracer {

  class Sphere {
  private:
    Transform _transform;
    Transform _inverse;

  public:
    Sphere(Transform t = Transform::id()) : _transform(t), _inverse(t.inverse()) {}

    const Transform &transform() const {
      return _transform;
    }

    const Transform &inverse() const {
      return _inverse;
    }

    std::vector<Intersection> intersect(Ray r);
  };

} // namespace RayTracer
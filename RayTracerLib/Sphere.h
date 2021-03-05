#pragma once

#include <vector>

#include "Object.h"
#include "Tuple.h"
#include "Ray.h"
#include "Intersection.h"
#include "Transformation.h"

namespace RayTracer {

  class Sphere : public Object {
  public:
    Sphere(Transform t = Transform::id(), Material m = Material()) : Object(t, m) {}

    virtual std::vector<Intersection> intersect(Ray r) override;

    virtual Vector normal(Point p) const override;
  };

} // namespace RayTracer
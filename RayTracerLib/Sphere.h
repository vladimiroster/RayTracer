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

  protected:
    virtual std::vector<Intersection> intersect_local(const Ray& r) const override;
    virtual Vector normal_local(const Point& p) const override;
  };

} // namespace RayTracer
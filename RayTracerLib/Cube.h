#pragma once

#include "Object.h"
#include "Tuple.h"

namespace RayTracer {

  class Cube : public Object {
  public:
    Cube(Transform t = Transform::id(), Material m = Material()) : Object(t, m) {}
    virtual std::vector<Intersection> intersect_local(const Ray& r) const;
    virtual Vector normal_local(const Point& r) const;
  };

} // namespace RayTracer
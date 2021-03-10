#pragma once

#include "Object.h"

namespace RayTracer {
  class Plane : public Object {
  public:
    Plane(Transform t = Transform::id(), Material m = Material()) : Object(t, m) {}

  protected:
    virtual std::vector<Intersection> intersect_local(const Ray& r) const;

    virtual Vector normal_local(const Point& r) const {
      return Vector(0, 1, 0);
    }

  };
} // namespace RayTracer
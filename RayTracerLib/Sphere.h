#pragma once

#include <vector>

#include "Tuple.h"
#include "Ray.h"
#include "Intersection.h"
#include "Transformation.h"
#include "Materials.h"

namespace RayTracer {

  class Sphere {
  public:
    Sphere(Transform t = Transform::id()) : _transform(t), _inverse(t.inverse()) {}

    const Transform &transform() const {
      return _transform;
    }

    const Transform &inverse() const {
      return _inverse;
    }

    const Material &material() const {
      return _material;
    }

    void set_material(const Material& m) {
      _material = m;
    }

    std::vector<Intersection> intersect(Ray r);

    Vector normal(Point p) const;

  private:
    Transform _transform;
    Transform _inverse;
    Material _material;
  };

} // namespace RayTracer
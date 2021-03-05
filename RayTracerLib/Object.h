#pragma once

#include "Transformation.h"
#include "Materials.h"
#include "Ray.h"
#include "Intersection.h"

namespace RayTracer {
  class Object {
  public:
    Object(Transform t = Transform::id(), Material m = Material()) : _transform(t), _inverse(t.inverse()), _material(m) {}

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

    virtual std::vector<Intersection> intersect(Ray r) = 0;

    virtual Vector normal(Point p) const = 0;

    // TODO: add virtual compare method for specific derived class comparisons
    bool operator==(const Object& rhs) const;
    bool operator!=(const Object& rhs) const;

  private:
    Transform _transform;
    Transform _inverse;

  protected:
    Material _material;
  };

} // namespace RayTracer
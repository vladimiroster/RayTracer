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

    std::vector<Intersection> intersect(Ray r) const {
      r = _inverse * r;
      return intersect_local(r);
    }

    virtual Vector normal(Point p) const {
      p = _inverse * p;
      auto loc_n = normal_local(p);
      auto w_n = _inverse.transpose() * loc_n;
      w_n.w = 0;
      return normalize(w_n);
    }

    // TODO: add virtual compare method for specific derived class comparisons
    bool operator==(const Object& rhs) const;
    bool operator!=(const Object& rhs) const;

  protected:
    virtual std::vector<Intersection> intersect_local(const Ray& r) const = 0;
    virtual Vector normal_local(const Point& r) const = 0;

  private:
    Transform _transform;
    Transform _inverse;

  protected:
    Material _material;
  };

} // namespace RayTracer
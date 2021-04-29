#pragma once

#include "Transformation.h"
#include "Materials.h"
#include "Ray.h"
#include "Intersection.h"

#include "../PhysicsLib/Behavior.h"

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

    std::shared_ptr<Physics::Behavior> behavior() {
      return _behavior;
    }

    void set_behavior(std::shared_ptr<Physics::Behavior> beh) {
      _behavior = beh;
    }

    void setup() {
      if (_behavior)
        _behavior->setup();
    }

    void act() {
      if (_behavior)
        _behavior->action();
    }

    void move(Transform transform) {
      // TODO: better way to do this?
      _transform = transform;
      _inverse = _transform.inverse();
    }

    // TODO: add virtual compare method for specific derived class comparisons
    bool operator==(const Object& rhs) const;
    bool operator!=(const Object& rhs) const;

//  protected: TODO: moved to public because of unit tests
    virtual std::vector<Intersection> intersect_local(const Ray& r) const = 0;
    virtual Vector normal_local(const Point& r) const = 0;

  private:
    Transform _transform;
    Transform _inverse;

  protected:
    Material _material;
    std::shared_ptr<Physics::Behavior> _behavior;
  };

} // namespace RayTracer
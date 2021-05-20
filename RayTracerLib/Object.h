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

    virtual ~Object() {
      std::cout << "Object is destroyed" << std::endl;
    }

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

    std::vector<std::shared_ptr<Physics::Behavior>>& behaviors() {
      return _behaviors;
    }

    const std::vector<std::shared_ptr<Physics::Behavior>>& behaviors() const {
      return _behaviors;
    }

    void setup() {
      for (auto b : _behaviors)
        b->setup();
    }

    void act() {
      for (auto b : _behaviors)
        _alive &= b->action();
    }

    void move(Transform transform) {
      // TODO: better way to do this?
      _transform = transform;
      _inverse = _transform.inverse();
    }

    bool isAlive() {
      return _alive;
    }

    // TODO: add virtual compare method for specific derived class comparisons
    bool operator==(const Object& rhs) const;
    bool operator!=(const Object& rhs) const;

//  protected: TODO: moved to public because of unit tests
    virtual std::vector<Intersection> intersect_local(const Ray& r) const = 0;
    virtual Vector normal_local(const Point& r) const = 0;

  private:
    // TODO: To allow rotation and scaling in later time, separate this into TRS values
    Transform _transform;
    Transform _inverse;

    bool _alive = true;

  protected:
    Material _material;
    std::vector<std::shared_ptr<Physics::Behavior>> _behaviors;
  };

} // namespace RayTracer
#pragma once

#include "Tuple.h"
#include "Matrix.h"

namespace RayTracer {
  
  class Ray {
  private:
    Point _o;
    Vector _d;

  public:
    Ray(Point p, Vector v) : _o(p), _d(v) {}

    const Point& origin() const {
      return _o;
    }

    const Vector& direction() const {
      return _d;
    }

    Point position(float t);
  };

  Ray operator*(const Matrix<4>& M, const Ray& r);

} // namespace RayTracer
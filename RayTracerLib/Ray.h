#pragma once

#include "Tuple.h"
#include "Matrix.h"

namespace RayTracer {
  
  struct Ray {
    Point origin;
    Vector direction;

    Ray(Point p, Vector v) : origin(p), direction(v) {}
    Point position(float t);
  };

  Ray operator*(const Matrix<4>& M, const Ray& r);

} // namespace RayTracer
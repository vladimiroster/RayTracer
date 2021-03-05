#include "Ray.h"

namespace RayTracer {

  RayTracer::Point RayTracer::Ray::position(float t)
  {
    return _o + _d * t;
  }

  RayTracer::Ray RayTracer::operator*(const Matrix<4>& M, const Ray & r)
  {
    return Ray(M * r.origin(), M * r.direction());
  }

} // namespace RayTracer
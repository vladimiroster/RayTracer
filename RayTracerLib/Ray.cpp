#include "Ray.h"

RayTracer::Point RayTracer::Ray::position(float t)
{
  return origin + direction * t;
}

RayTracer::Ray RayTracer::operator*(const Matrix<4>& M, const Ray & r)
{
  return Ray(M * r.origin, M * r.direction);
}

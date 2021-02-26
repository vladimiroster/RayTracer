#include "Sphere.h"

std::vector<RayTracer::Intersection> RayTracer::Sphere::intersect(Ray r)
{
  r = _inverse * r;

  // Note: sphere center is at world origin
  auto s2r = r.origin() - Point(0, 0, 0);

  // TODO: precompute either in ray, or even better in the immutable vector as norm
  auto a = dot(r.direction(), r.direction());
  auto b = 2 * dot(r.direction(), s2r);
  auto c = dot(s2r, s2r) -1;

  auto discr = b * b - 4 * a * c;

  if (discr < 0) {
    return std::vector<Intersection>();
  } else {
    // Note: if d = 0, we're returning the same time value twice
    return std::vector<Intersection>({Intersection((-b - sqrtf(discr))/(2 * a), *this), 
      Intersection((-b + sqrtf(discr))/(2 * a), *this)});
  }
}

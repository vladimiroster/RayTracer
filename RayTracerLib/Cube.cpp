#include "Cube.h"

#include <limits>

namespace RayTracer {

  std::pair<float, float> check_axis(float origin, float direction) {
    auto tmin_numerator = (-1 - origin);
    auto tmax_numerator = (1 - origin);

    float tmin = 0;
    float tmax = 0;

    if (fabsf(direction) >= epsilon) {
      tmin = tmin_numerator / direction;
      tmax = tmax_numerator / direction;
    }
    else {
      tmin = tmin_numerator * FLT_MAX;
      tmax = tmax_numerator * FLT_MAX;
    }

    if (tmin > tmax) {
      return std::make_pair(tmax, tmin);
    }
    else {
      return std::make_pair(tmin, tmax);
    }
  }

  std::vector<Intersection> RayTracer::Cube::intersect_local(const Ray & r) const
  {
    auto xt = check_axis(r.origin().x, r.direction().x);
    auto yt = check_axis(r.origin().y, r.direction().y);
    auto zt = check_axis(r.origin().z, r.direction().z);

    auto tmin = std::max({xt, yt, zt}, [](std::pair<float, float> t1, std::pair<float, float> t2) { return t1.first < t2.first;}).first;
    auto tmax = std::min({xt, yt, zt}, [](std::pair<float, float> t1, std::pair<float, float> t2) { return t1.second < t2.second;}).second;

    if (tmin > tmax) {
      return {};
    }

    return std::vector<Intersection>({Intersection(tmin, *this), Intersection(tmax, *this)});
  }

  Vector RayTracer::Cube::normal_local(const Point & p) const
  {
    auto maxc = std::max({fabsf(p.x), fabsf(p.y), fabsf(p.z)});

    if (maxc == fabsf(p.x)) {
      return Vector(p.x, 0, 0);
    } else if (maxc == fabsf(p.y)) {
      return Vector(0, p.y, 0);
    } 
    return Vector(0, 0, p.z);
  }

} // namespace RayTracer
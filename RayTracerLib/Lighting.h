#pragma once

#include "Tuple.h"

namespace RayTracer {

  // TODO: when more is known about lights, have Light be the abstract class and derive PointLight from it
  struct Light {
  public:
    Light(Color i, Point p) : intensity(i), position(p) {}

    Color intensity;
    Point position;
  };

  bool operator==(const Light& lhs, const Light& rhs);
  bool operator!=(const Light& lhs, const Light& rhs);

} // namespace RayTracer

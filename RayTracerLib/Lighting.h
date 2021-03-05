#pragma once

#include "Tuple.h"

namespace RayTracer {
  struct PointLight {
  public:
    PointLight(Color i, Point p) : intensity(i), position(p) {}

    Color intensity;
    Point position;
  };

} // namespace RayTracer

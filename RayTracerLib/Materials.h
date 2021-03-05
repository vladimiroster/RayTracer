#pragma once

#include "Tuple.h"
#include "Lighting.h"

namespace RayTracer {
  struct Material {
    Material() :
      color(1, 1, 1), ambient(0.1f), diffuse(0.9f), specular(0.9f), shininess(200) {}

    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
  };

  bool operator==(const Material& lhs, const Material& rhs);
  bool operator!=(const Material& lhs, const Material& rhs);

  Color lighting(Material m, PointLight light, Point p, Vector eyev, Vector normalv);
} // namespace RayTracer

#pragma once

#include "Tuple.h"
#include "Lighting.h"

namespace RayTracer {
  struct Material {
    Material() :
      color(1, 1, 1), ambient(0.1f), diffuse(0.9f), specular(0.9f), shininess(200) {}
    Material(Color c, float a, float d, float s, float sh) :
      color(c), ambient(a), diffuse(d), specular(s), shininess(sh) {}

    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
  };

  bool operator==(const Material& lhs, const Material& rhs);
  bool operator!=(const Material& lhs, const Material& rhs);

  Color lighting(Material m, Light light, Point p, Vector eyev, Vector normalv);
} // namespace RayTracer

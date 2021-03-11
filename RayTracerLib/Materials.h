#pragma once

#include "Tuple.h"
#include "Lighting.h"
#include "Pattern.h"

namespace RayTracer {
  struct Material {
    Material() :
      color(1, 1, 1), ambient(0.1f), diffuse(0.9f), specular(0.9f), shininess(200) {}
    Material(Color c, float a, float d, float s, float sh, std::shared_ptr<Pattern> pat = nullptr) :
      color(c), ambient(a), diffuse(d), specular(s), shininess(sh), pattern(std::move(pat)) {}

    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    std::shared_ptr<Pattern> pattern;
  };

  bool operator==(const Material& lhs, const Material& rhs);
  bool operator!=(const Material& lhs, const Material& rhs);

  Color lighting(Material m, std::reference_wrapper<const Object> obj, Light light, Point p, Vector eyev, Vector normalv, bool in_shadow);
} // namespace RayTracer

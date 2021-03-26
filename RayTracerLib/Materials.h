#pragma once

#include "Tuple.h"
#include "Lighting.h"
#include "Pattern.h"

namespace RayTracer {
  struct Material {
    Material() :
      color(white), ambient(0.1f), diffuse(0.9f), specular(0.9f), shininess(200), reflective(0.0f), transparency(0.0f), refractive_index(1.0f) {}
    Material(Color c, float a, float d, float s, float sh, float r, float t, float ri, std::shared_ptr<Pattern> pat = nullptr) :
      color(c), ambient(a), diffuse(d), specular(s), shininess(sh), reflective(r), transparency(t), refractive_index(ri), pattern(std::move(pat)) {}

    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    float reflective;
    float transparency;
    float refractive_index;
    std::shared_ptr<Pattern> pattern;
  };

  inline const float vacuum = 1;
  inline const float air = 1.00029f;
  inline const float water = 1.333f;
  inline const float glass = 1.52f;
  inline const float diamond = 2.417f;

  bool operator==(const Material& lhs, const Material& rhs);
  bool operator!=(const Material& lhs, const Material& rhs);

  Color lighting(Material m, std::reference_wrapper<const Object> obj, Light light, Point p, Vector eyev, Vector normalv, bool in_shadow);
} // namespace RayTracer

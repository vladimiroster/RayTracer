#pragma once

#include <iostream>

namespace RayTracer {
  constexpr float epsilon = 0.00001f;

  inline bool ApproxEqual(float a, float b, float epsilon) {
    return fabs(a - b) < epsilon;
  }

  struct Tuple {
    float x,y,z,w;

    Tuple(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1) {}
  };

  struct Point : public Tuple {
    Point(float x1, float y1, float z1) : Tuple(x1, y1, z1, 1.0f) {}
    Point(Tuple&& t) : Point(t.x, t.y, t.z) {}
  };

  struct Vector : public Tuple {
    Vector(float x1, float y1, float z1) : Tuple(x1, y1, z1, 0.0f) {}
    Vector(Tuple&& t) : Vector(t.x, t.y, t.z) {}
  };

  struct Color : public Tuple {
    Color() : Tuple(0.0f, 0.0f, 0.0f, 0.0f) {}
    Color(float r, float g, float b) : Tuple(r, g, b, 0.0f) {}
  };

  bool operator==(const Tuple& lhs, const Tuple& rhs);
  std::ostream& operator<<(std::ostream& os, const Tuple& t);
  Tuple operator-(const Tuple& a);
  Tuple operator+(const Tuple& lhs, const Tuple& rhs);
  Tuple operator-(const Tuple& lhs, const Tuple& rhs);
  Tuple operator*(const Tuple& lhs, float scalar);
  Tuple operator/(const Tuple& lhs, float scalar);
  float magnitude(const Tuple& tpl);
  Tuple normalize(const Tuple& tpl);
  float dot(const Tuple& lhs, const Tuple& rhs);
  Vector cross(const Vector& lhs, const Vector& rhs);
  // Hadamard product (Schur product)
  Color operator*(const Color& lhs, const Color& rhs);

} // namespace RayTracer
#pragma once

#include <array>
#include <iostream>

namespace RayTracer {
  constexpr float epsilon = 0.0001f;

  inline bool ApproxEqual(float a, float b, float epsilon) {
    return fabs(a - b) < epsilon;
  }

  struct Tuple {
    float x,y,z,w;

    Tuple(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1) {}
    Tuple(std::array<float, 4> arr) {
      x = arr[0];
      y = arr[1];
      z = arr[2];
      w = arr[3];
    }

    float operator[](size_t idx) {
      switch (idx) {
      case 0: return x;
        break;
      case 1: return y;
        break;
      case 2: return z;
        break;
      case 4: return w;
        break;
      default:
        throw std::invalid_argument("Tuple has only 4 data members");
      }
    }
  };

  inline const Tuple zero(0, 0, 0, 0);

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
    Color(Tuple&& t) : Color(t.x, t.y, t.z) {}
  };

  inline const Color black(0, 0, 0);
  inline const Color white(1, 1, 1);

  bool operator==(const Tuple& lhs, const Tuple& rhs);
  bool operator!=(const Tuple& lhs, const Tuple& rhs);
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
  Vector reflect(const Vector& in, const Vector& normal);

} // namespace RayTracer
#pragma once

namespace RayTracer {
  constexpr float epsilon = 0.00001f;

  bool ApproxEqual(float a, float b, float epsilon) {
    return fabs(a - b) < epsilon;
  }

  struct Tuple {
    float x,y,z,w;

    Tuple(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1) {}
  };

  struct Point : public Tuple {
    Point(float x1, float y1, float z1) : Tuple(x1, y1, z1, 1.0f) {}
  };

  struct Vector : public Tuple {
    Vector(float x1, float y1, float z1) : Tuple(x1, y1, z1, 0.0f) {}
  };

  bool operator==(const Tuple& lhs, const Tuple& rhs) {
    return ApproxEqual(lhs.x, rhs.x, epsilon) &&
      ApproxEqual(lhs.y, rhs.y, epsilon) &&
      ApproxEqual(lhs.z, rhs.z, epsilon) &&
      ApproxEqual(lhs.w, rhs.w, epsilon);
  }

  Tuple operator-(const Tuple& a) {
    return Tuple(-a.x, -a.y, -a.z, -a.w);
  }

  Tuple operator+(const Tuple& lhs, const Tuple& rhs) {
    return Tuple(lhs.x + rhs.x,
      lhs.y + rhs.y,
      lhs.z + rhs.z,
      lhs.w + rhs.w);
  }

  Tuple operator-(const Tuple& lhs, const Tuple& rhs) {
    return lhs + (-rhs);
  }

  Tuple operator*(const Tuple& lhs, float scalar) {
    return Tuple(lhs.x * scalar,
      lhs.y * scalar,
      lhs.z * scalar,
      lhs.w * scalar);
  }

  Tuple operator/(const Tuple& lhs, float scalar) {
    return Tuple(lhs.x / scalar,
      lhs.y / scalar,
      lhs.z / scalar,
      lhs.w / scalar);
  }

  float magnitude(const Tuple& tpl) {
    return sqrtf(pow(tpl.x, 2) + pow(tpl.y, 2) + pow(tpl.z, 2) + pow(tpl.w, 2));
  }

  Tuple normalize(const Tuple& tpl) {
    return tpl / magnitude(tpl);
  }

  float dot(const Tuple& lhs, const Tuple& rhs) {
    return lhs.x * rhs.x +
      lhs.y * rhs.y +
      lhs.z * rhs.z +
      lhs.w * rhs.w;
  }

  Vector cross(const Vector& lhs, const Vector& rhs) {
    return Vector(lhs.y * rhs.z - lhs.z * rhs.y,
      lhs.z * rhs.x - lhs.x * rhs.z,
      lhs.x * rhs.y - lhs.y * rhs.x);
  }
}
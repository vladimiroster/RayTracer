#include "Tuple.h"

namespace RayTracer {

  bool operator==(const Tuple& lhs, const Tuple& rhs) {
    return ApproxEqual(lhs.x, rhs.x, epsilon) &&
      ApproxEqual(lhs.y, rhs.y, epsilon) &&
      ApproxEqual(lhs.z, rhs.z, epsilon) &&
      ApproxEqual(lhs.w, rhs.w, epsilon);
  }

  bool operator!=(const Tuple& lhs, const Tuple& rhs) {
    return !(lhs == rhs);
  }

  std::ostream& operator<<(std::ostream& os, const Tuple& t) {
    os << "(" << t.x << ", " << t.y << ", " << t.z << ", " << t.w << ")";
    return os;
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

  // Hadamard product (Schur product)
  Color operator*(const Color& lhs, const Color& rhs) {
    return Color(lhs.x * rhs.x, lhs.y  * rhs.y, lhs.z * rhs.z);
  }

  Vector reflect(const Vector & in, const Vector & normal)
  {
    return in - normal * 2 * dot(in, normal);
  }
} // namespace RayTracer
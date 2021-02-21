#include "Transformation.h"

namespace RayTracer {
  Transform Transform::id()
  {
    return Transform(id4);
  }

  Transform Transform::translate(float x, float y, float z)
  {
    auto t = id4;
    t[0][3] = x;
    t[1][3] = y;
    t[2][3] = z;
    return Transform(t) * *this;
  }

  Transform Transform::scale(float x, float y, float z)
  {
    auto t = id4;
    t[0][0] = x;
    t[1][1] = y;
    t[2][2] = z;
    return Transform(t) * *this;
  }

  Transform Transform::rot_x(float r)
  {
    auto t = id4;
    t[1][1] = std::cosf(r);
    t[1][2] = -std::sinf(r);
    t[2][1] = std::sinf(r);
    t[2][2] = std::cosf(r);
    return Transform(t) * *this;
  }

  Transform Transform::rot_y(float r)
  {
    auto t = id4;
    t[0][0] = std::cosf(r);
    t[0][2] = std::sinf(r);
    t[2][0] = -std::sinf(r);
    t[2][2] = std::cosf(r);
    return Transform(t) * *this;
  }

  Transform Transform::rot_z(float r)
  {
    auto t = id4;
    t[0][0] = std::cosf(r);
    t[0][1] = -std::sinf(r);
    t[1][0] = std::sinf(r);
    t[1][1] = std::cosf(r);
    return Transform(t) * *this;
  }

  Transform Transform::shear(float xy, float xz, float yx, float yz, float zx, float zy)
  {
    auto t = id4;
    t[0][1] = xy;
    t[0][2] = xz;
    t[1][0] = yx;
    t[1][2] = yz;
    t[2][0] = zx;
    t[2][1] = zy;
    return Transform(t) * *this;
  }

} // namespace RayTracer
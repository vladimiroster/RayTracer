#pragma once

#include "Matrix.h"

namespace RayTracer {

  class Transform : public Matrix<4> {
  public:
    Transform() : Transform(Transform::id()) {}
    Transform(const Matrix<4>& m) : Matrix<4>(m) {}

    static Transform id();
    Transform translate(float x, float y, float z) const;
    Transform scale(float x, float y, float z) const;
    // TODO: add arbitrary quaternion rotations
    Transform rot_x(float r) const;
    Transform rot_y(float r) const;
    Transform rot_z(float r) const;
    Transform shear(float xy, float xz, float yx, float yz, float zx, float zy) const;
    Transform view(Point from, Point to, Vector up) const;
  };

} // namespace RayTracer
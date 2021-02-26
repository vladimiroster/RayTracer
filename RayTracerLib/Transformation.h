#pragma once

#include "Matrix.h"

namespace RayTracer {

  class Transform : public Matrix<4> {
  public:
    Transform(const Matrix<4>& m) : Matrix<4>(m) {}

    static Transform id();
    Transform translate(float x, float y, float z);
    Transform scale(float x, float y, float z);
    // TODO: add arbitrary quaternion rotations
    Transform rot_x(float r);
    Transform rot_y(float r);
    Transform rot_z(float r);
    Transform shear(float xy, float xz, float yx, float yz, float zx, float zy);
  };

} // namespace RayTracer
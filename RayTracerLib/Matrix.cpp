#include "Matrix.h"

namespace RayTracer {

  Tuple operator*(const Matrix<4>& lhs, const Tuple& rhs) {
    return Tuple(dot(lhs[0], rhs),
      dot(lhs[1], rhs),
      dot(lhs[2], rhs),
      dot(lhs[3], rhs));
  }

} // namespace RayTracer
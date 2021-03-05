#include "Lighting.h"

namespace RayTracer {

  bool operator==(const Light& lhs, const Light& rhs) {
    return lhs.intensity == rhs.intensity && lhs.position == rhs.position;
  }

  bool operator!=(const Light& lhs, const Light& rhs) {
    return !(lhs == rhs);
  }

} // namespace RayTracer

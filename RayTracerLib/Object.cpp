#include "Object.h"

namespace RayTracer {
  bool Object::operator==(const Object& rhs) const
  {
    return _transform == rhs._transform && _material == rhs._material;
  }

  bool Object::operator!=(const Object& rhs) const
  {
    return !(*this == rhs);
  }
} // namespace RayTracer
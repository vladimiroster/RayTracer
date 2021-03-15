#include "Pattern.h"

#include "Object.h"

namespace RayTracer {
  Color Pattern::color_at_object(std::reference_wrapper<const Object> o, Point p) const
  {
    // World space -> object space
    auto obj_p = o.get().inverse() * p;
    // Object space -> pattern space
    Point pat_p = _inverse * obj_p;

    return color_at(pat_p);
  }

  Color StripePattern::color_at(Point p) const
  {
    if (static_cast<long long>(floorf(p.x)) % 2 == 0) {
      return _c1;
    }
    else {
      return _c2;
    }
  }


} // namespace RayTracer
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

  Color SolidPattern::color_at(Point p) const
  {
    return _c;
  }

  Color StripePattern::color_at(Point p) const
  {
    if (static_cast<long long>(floorf(p.x)) % 2 == 0) {
      return _p1->color_at(p);
    }
    return _p2->color_at(p);
  }

  // TODO: doesn't look too nice
  //Color GradientPattern::color_at(Point p) const
  //{
  //  float fraction = p.x - floorf(p.x);
  //  auto retval = _c1.color() + _distance * fraction;
  //  return retval;
  //}
   
  Color RingPattern::color_at(Point p) const
  {
    if (static_cast<int>(floorf(sqrtf(p.x * p.x + p.z * p.z))) % 2 == 0) {
      return _p1->color_at(p);
    }
    return _p2->color_at(p);
  }

  Color CheckersPattern::color_at(Point p) const
  {
    if (static_cast<long long>(floorf(p.x) + floorf(p.y) + floorf(p.z)) % 2 == 0) {
      return _p1->color_at(p);
    }
    return _p2->color_at(p);
  }

  Color BlendedPattern::color_at(Point p) const
  {
    return _blender(_p1->color_at(p), _p2->color_at(p));
  }

} // namespace RayTracer
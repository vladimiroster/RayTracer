#pragma once

#include <utility>

#include "Transformation.h"

namespace RayTracer {
  class Object;

  class Pattern {
  public:
    Pattern(Transform transform = Transform::id()) : _transform(transform), _inverse(transform.inverse()) {}

    virtual Color color_at(Point p) const = 0;

    Color color_at_object(std::reference_wrapper<const Object> o, Point p) const;
    
    const Transform& transform() const {
      return _transform;
    }

  private:
    Transform _transform;
    Transform _inverse;
  };

  class StripePattern : public Pattern {
  public:
    StripePattern(Color a, Color b, Transform transform = Transform::id()) : _c1(a), _c2(b), Pattern(transform) {}

    virtual Color color_at(Point p) const override;

    std::pair<Color, Color> colors() const {
      return std::make_pair(_c1, _c2);
    }
  private:
    Color _c1;
    Color _c2;
  };

} // namespace RayTracer
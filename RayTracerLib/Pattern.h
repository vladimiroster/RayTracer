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

  class TwoColorPattern : public Pattern {
  public:
    TwoColorPattern(Color a, Color b, Transform transform = Transform::id()) : _c1(a), _c2(b), Pattern(transform) {}

    std::pair<Color, Color> colors() const {
      return std::make_pair(_c1, _c2);
    }
  protected:
    Color _c1;
    Color _c2;
  };

  class StripePattern : public TwoColorPattern {
  public:
    StripePattern(Color a, Color b, Transform transform = Transform::id()) : TwoColorPattern(a, b, transform) {}

    virtual Color color_at(Point p) const override;
  };

  class GradientPattern : public TwoColorPattern {
  public:
    GradientPattern(Color a, Color b, Transform transform = Transform::id()) : TwoColorPattern(a, b, transform), _distance(b - a) {}

    virtual Color color_at(Point p) const override;

  private:
    Color _distance;
  };
} // namespace RayTracer
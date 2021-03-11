#pragma once

#include <utility>

#include "Transformation.h"

namespace RayTracer {
  class Object;

  class Pattern {
  public:
    Pattern(Color a, Color b, Transform transform = Transform::id()) : _c1(a), _c2(b), _transform(transform), _inverse(transform.inverse()) {}

    Color color_at(Point p) const;

    Color color_at_object(std::reference_wrapper<const Object> o, Point p) const;
    
    std::pair<Color, Color> colors() const {
      return std::make_pair(_c1, _c2);
    }

    const Transform& transform() const {
      return _transform;
    }

  private:
    Color _c1;
    Color _c2;
    Transform _transform;
    Transform _inverse;
  };

} // namespace RayTracer
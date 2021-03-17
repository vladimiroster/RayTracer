#pragma once

#include <utility>
#include <functional>

#include "Transformation.h"

namespace RayTracer {
  class Object;

  // TODO:
  // 1. Radial gradient
  // 2. Perturbed pattern (use 3D noise (Perlin, Simplex) to jitter the point)
  // 3. Test and beautify patterns
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

  class SolidPattern : public Pattern {
  public:
    SolidPattern(Color a) : _c(a) {}

    virtual Color color_at(Point p) const override;

    Color color() const {
      return _c;
    }

  private:
    Color _c;
  };

  class TwoComponentPattern : public Pattern {
  public:
    TwoComponentPattern(std::shared_ptr<Pattern> a, std::shared_ptr<Pattern> b, Transform transform = Transform::id()) : _p1(a), _p2(b), Pattern(transform) {}

  protected:
    std::shared_ptr<Pattern> _p1;
    std::shared_ptr<Pattern> _p2;
  };

  class StripePattern : public TwoComponentPattern {
  public:
    StripePattern(std::shared_ptr<Pattern> a, std::shared_ptr<Pattern> b, Transform transform = Transform::id()) : TwoComponentPattern(a, b, transform) {}

    virtual Color color_at(Point p) const override;
  };

  //class GradientPattern : public TwoColorPattern {
  //public:
  //  GradientPattern(Color a, Color b, Transform transform = Transform::id()) : TwoColorPattern(a, b, transform), _distance(b - a) {}

  //  virtual Color color_at(Point p) const override;

  //private:
  //  Color _distance;
  //};

  class RingPattern : public TwoComponentPattern {
  public:
    RingPattern(std::shared_ptr<Pattern> a, std::shared_ptr<Pattern> b, Transform transform = Transform::id()) : TwoComponentPattern(a, b, transform) {}

    virtual Color color_at(Point p) const override;
  };

  // TODO: Spherical texture mapping (UV mapping)
  class CheckersPattern : public TwoComponentPattern {
  public:
    CheckersPattern(std::shared_ptr<Pattern> a, std::shared_ptr<Pattern> b, Transform transform = Transform::id()) : TwoComponentPattern(a, b, transform) {}

    virtual Color color_at(Point p) const override;
  };

  class BlendedPattern : public TwoComponentPattern {
  public:
    BlendedPattern(std::shared_ptr<Pattern> a, std::shared_ptr<Pattern> b, std::function<Color(Color, Color)> blender = [](Color a, Color b) { return (a + b)/2; },Transform transform = Transform::id()) : TwoComponentPattern(a, b, transform), _blender(blender) {}

    virtual Color color_at(Point p) const override;
  private:
    std::function<Color(Color, Color)> _blender;
  };

} // namespace RayTracer
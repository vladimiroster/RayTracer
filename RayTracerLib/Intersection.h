#pragma once

#include <vector>
#include <optional>
#include <functional>

namespace RayTracer {
  class Sphere;

  class Intersection {
  private:
    float _t;
    // TODO: Handle scope problem. Maybe use weak pointer?
    std::reference_wrapper<const Sphere> _s;

  public:
    Intersection(float time, const Sphere& sph) : _t(time), _s(sph) {}

    float time() const {
      return _t;
    }

    const Sphere& sphere() const {
      return _s.get();
    }

    bool operator==(const Intersection& rhs) const;
    bool operator!=(const Intersection& rhs) const;
  };


  std::optional<Intersection> hit(const std::vector<Intersection>& xs);

} // namespace RayTracer
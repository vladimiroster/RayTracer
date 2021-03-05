#pragma once

#include <vector>
#include <optional>
#include <functional>

namespace RayTracer {
  class Object;

  class Intersection {
  private:
    float _t;
    // TODO: Handle scope problem. Maybe use weak pointer?
    std::reference_wrapper<const Object> _o;

  public:
    Intersection(float time, const Object& obj) : _t(time), _o(obj) {}

    float time() const {
      return _t;
    }

    const Object& object() const {
      return _o.get();
    }

    bool operator==(const Intersection& rhs) const;
    bool operator!=(const Intersection& rhs) const;
  };


  std::optional<Intersection> hit(const std::vector<Intersection>& xs);

} // namespace RayTracer
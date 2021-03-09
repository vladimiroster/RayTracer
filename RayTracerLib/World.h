#pragma once

#include <vector>
#include <memory>

#include "Object.h"
#include "Lighting.h"

namespace RayTracer {

  class World {
  public:
    std::vector<std::shared_ptr<Object>>& objects() {
      return _objects;
    }

    const std::vector<std::shared_ptr<Object>>& objects() const {
      return _objects;
    }

    std::vector<std::shared_ptr<Light>>& lights() {
      return _lights;
    }

    const std::vector<std::shared_ptr<Light>>& lights() const {
      return _lights;
    }

    static std::unique_ptr<World> default_world();

    std::vector<Intersection> intersect(Ray r) const;

    Color shade_hit(Intersection::Computation comps) const;

    Color color_at(const Ray& r) const;

  private:
    std::vector<std::shared_ptr<Object>> _objects;
    std::vector<std::shared_ptr<Light>> _lights;
  };
} //namespace RayTracer
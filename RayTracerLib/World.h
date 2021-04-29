#pragma once

#include <vector>
#include <memory>

#include "Object.h"
#include "Lighting.h"

#include "../PhysicsLib/Engine.h"

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

    Color shade_hit(const Intersection::Computation& comps, size_t remaining = 4) const;

    Color color_at(const Ray& r, size_t remaining = 4) const;

    bool is_shadowed(Point p) const;

    Color reflected_color(const Intersection::Computation& comps, size_t remaining = 4) const;

    Color refracted_color(const Intersection::Computation& comps, size_t remaining = 4) const;

    void setup();

    void act();

  private:
    std::vector<std::shared_ptr<Object>> _objects;
    std::vector<std::shared_ptr<Light>> _lights;
    std::unique_ptr<Physics::Engine> _physics = std::make_unique<Physics::Engine>(Vector(0, -0.09f, 0));
  };
} //namespace RayTracer
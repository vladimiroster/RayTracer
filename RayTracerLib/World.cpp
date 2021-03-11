#include "World.h"

#include "Sphere.h"
#include "Ray.h"

namespace RayTracer {

  std::unique_ptr<World> World::default_world()
  {
    auto w = std::make_unique<World>();
    w->lights().emplace_back(std::make_shared<Light>(Color(1, 1, 1), Point(-10, 10, -10)));
    w->objects().emplace_back(std::make_shared<Sphere>(Transform::id(), Material(Color(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f, 200)));
    w->objects().emplace_back(std::make_shared<Sphere>(Transform::id().scale(0.5f, 0.5f, 0.5f)));
    return w;
  }

  std::vector<Intersection> World::intersect(Ray r) const
  {
    std::vector<Intersection> xs;
    // Allocate space for maximum amount of intersections
    xs.reserve(_objects.size() * 2);

    for (auto obj : _objects) {
      auto obj_xs = obj->intersect(r);
      xs.insert(xs.end(), obj_xs.begin(), obj_xs.end());
    }
    // Sorting here because the book says we'll need that in reflection and CSG
    // However, I'm already making a heap in the "hit" function. Once all the requirements
    // are clear, we only need one of these operations
    std::sort(xs.begin(), xs.end(), [] (const Intersection& x1, const Intersection& x2) { return x1.time() < x2.time(); });
    return xs;
  }

  Color World::shade_hit(Intersection::Computation comps) const
  {
    // TODO: support multiple light sources
    return lighting(comps.object.get().material(), comps.object, *_lights.front(), comps.over_point, comps.eyev, comps.normalv, is_shadowed(comps.over_point));
  }

  Color World::color_at(const Ray & r) const
  {
    auto xs = intersect(r);
    auto i = hit(xs);
    if (i) {
      return shade_hit(i->precompute(r));
    }
    else {
      return black;
    }
  }

  bool World::is_shadowed(Point p) const
  {
    // TODO: multiple light sources
    auto point_to_light = _lights.front()->position - p;
    auto distance = magnitude(point_to_light);
    Ray r(p, normalize(point_to_light));
    auto xs = intersect(r);
    auto h = hit(xs);
    if (h && h->time() < distance) {
      return true;
    }
    return false;
  }

} //namespace RayTracer

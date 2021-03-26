#include "World.h"

#include "Sphere.h"
#include "Ray.h"

namespace RayTracer {

  std::unique_ptr<World> World::default_world()
  {
    auto w = std::make_unique<World>();
    w->lights().emplace_back(std::make_shared<Light>(Color(1, 1, 1), Point(-10, 10, -10)));
    w->objects().emplace_back(std::make_shared<Sphere>(Transform::id(), Material(Color(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f, 200, 0, 0, 1)));
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

  Color World::shade_hit(const Intersection::Computation& comps, size_t remaining) const
  {
    Color c = black;
    for (auto l : _lights) {
      c = c + lighting(comps.object.get().material(), comps.object, *l, comps.over_point, comps.eyev, comps.normalv, is_shadowed(comps.over_point));
    }
    // Add reflection and refraction
    // Account for total internal reflection
    auto reflected = reflected_color(comps, remaining);
    auto refracted = refracted_color(comps, remaining);

    if (comps.object.get().material().reflective > 0 && comps.object.get().material().transparency > 0) {
      auto reflectance = schlick(comps);
      return c + reflected * reflectance + refracted * (1 - reflectance);
    }
    else {
      return c + reflected + refracted;
    }
  }

  Color World::color_at(const Ray & r, size_t remaining) const
  {
    auto xs = intersect(r);
    auto i = hit(xs);
    if (i) {
      return shade_hit(i->precompute(r, xs), remaining);
    }
    else {
      return black;
    }
  }

  bool World::is_shadowed(Point p) const
  {
    // TODO: write tests for this, not working perfectly
    for (auto l : _lights) {
      auto point_to_light = l->position - p;
      auto distance = magnitude(point_to_light);
      Ray r(p, normalize(point_to_light));
      auto xs = intersect(r);
      auto h = hit(xs);
      if (h && h->time() < distance) {
        return true;
      }
    }
    return false;
  }

  Color World::reflected_color(const Intersection::Computation& comps, size_t remaining) const
  {
    if (remaining < 1) {
      return black;
    }
    if (comps.object.get().material().reflective == 0.0f) {
      return black;
    }
    Ray reflect_ray(comps.over_point, comps.reflectv);
    auto c = color_at(reflect_ray, remaining - 1);

    return c * comps.object.get().material().reflective;
  }

  Color World::refracted_color(const Intersection::Computation& comps, size_t remaining) const
  {
    if (remaining == 0) {
      return black;
    }
    if (comps.object.get().material().transparency == 0) {
      return black;
    }

    // Evaluate total internal reflection
    auto n_ratio = comps.n1 / comps.n2;
    auto cos_i = dot(comps.eyev, comps.normalv);
    auto sin2_t = n_ratio * n_ratio * (1 - cos_i * cos_i);
    if (sin2_t > 1) {
      // Indeed
      return black;
    }

    auto cos_t = sqrtf(1.0f - sin2_t);

    // Compute the direction of the refracted ray
    Vector direction = comps.normalv * (n_ratio * cos_i - cos_t) - comps.eyev * n_ratio;
    Ray refract_ray(comps.under_point, direction);

    auto color = color_at(refract_ray, remaining - 1) * comps.object.get().material().transparency;

    return color;
  }

} //namespace RayTracer

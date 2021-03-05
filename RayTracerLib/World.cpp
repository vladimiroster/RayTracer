#include "World.h"

#include "Sphere.h"

namespace RayTracer {

  std::unique_ptr<World> World::default_world()
  {
    auto w = std::make_unique<World>();
    w->lights().emplace_back(std::make_shared<Light>(Color(1, 1, 1), Point(-10, 10, -10)));
    w->objects().emplace_back(std::make_shared<Sphere>(Transform::id(), Material(Color(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f, 200)));
    w->objects().emplace_back(std::make_shared<Sphere>(Transform::id().scale(0.5f, 0.5f, 0.5f)));
    return w;
  }

} //namespace RayTracer

#pragma once

#include "Tuple.h"

namespace rt = RayTracer;

struct Projectile {
  rt::Point position;
  rt::Vector velocity;

  Projectile(rt::Point pos, rt::Vector vel) : position(pos), velocity(vel) {}
};

struct Environment {
  rt::Vector gravity;
  rt::Vector wind;

  Environment(rt::Vector g, rt::Vector w) : gravity(g), wind(w) {}
};

Projectile tick(const Environment& env, const Projectile& p) {
  auto proj = Projectile(p.position + p.velocity, p.velocity + env.gravity + env.wind);
  return proj;
}
#include "pch.h"

#include "../RayTracerLib/World.h"
#include "../RayTracerLib/Lighting.h"
#include "../RayTracerLib/Sphere.h"

using namespace RayTracer;

TEST(TestWorld, TestWorldCreate) {
  World w;
  EXPECT_EQ(0, w.objects().size());
  EXPECT_EQ(0, w.lights().size());
}

TEST(TestWorld, TestDefaultWorld) {
  auto w = World::default_world();

  Light l(Color(1, 1, 1), Point(-10, 10, -10));
  Sphere s1;
  Material m;
  m.color = Color(0.8f, 1.0f, 0.6f);
  m.diffuse = 0.7f;
  m.specular = 0.2f;
  s1.set_material(m);

  Sphere s2(Transform::id().scale(0.5f, 0.5f, 0.5f));

  ASSERT_EQ(1, w->lights().size());
  EXPECT_EQ(l, *(w->lights().front()));
  ASSERT_EQ(2, w->objects().size());
  EXPECT_EQ(s1, *w->objects()[0]);
  EXPECT_EQ(s2, *w->objects()[1]);
}

TEST(TestWorld, TestWorldRayIntersect) {
  auto w = World::default_world();
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));

  auto xs = w->intersect(r);
  ASSERT_EQ(4, xs.size());
  EXPECT_EQ(4, xs[0].time());
  EXPECT_EQ(4.5, xs[1].time());
  EXPECT_EQ(5.5, xs[2].time());
  EXPECT_EQ(6, xs[3].time());
}

TEST(TestWorld, TestShadeIntersection) {
  auto w = World::default_world();
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  auto shape = w->objects().front();
  Intersection i(4, *shape);
  auto comps = i.precompute(r);
  auto c = w->shade_hit(comps);
  ASSERT_EQ(Color(0.38066f, 0.47583f, 0.2855f), c);
}

TEST(TestWorld, TestShadeIntersectionInside) {
  auto w = World::default_world();
  w->lights()[0] = std::make_shared<Light>(Color(1, 1, 1), Point(0, 0.25f, 0));
  Ray r(Point(0, 0, 0), Vector(0, 0, 1));
  auto shape = w->objects()[1];
  Intersection i(0.5, *shape);
  auto comps = i.precompute(r);
  auto c = w->shade_hit(comps);
  ASSERT_EQ(Color(0.90498f, 0.90498f, 0.90498f), c);
}

TEST(TestWorld, TestColorMiss) {
  auto w = World::default_world();
  Ray r(Point(0, 0, -5), Vector(0, 1, 0));
  ASSERT_EQ(Color(0, 0, 0), w->color_at(r));
}

TEST(TestWorld, TestColorHit) {
  auto w = World::default_world();
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  ASSERT_EQ(Color(0.38066f, 0.47583f, 0.2855f), w->color_at(r));
}

TEST(TestWorld, TestColorBehindRay) {
  auto w = World::default_world();
  auto outer = w->objects().front();
  auto m = outer->material();
  m.ambient = 1;
  outer->set_material(m);
  auto inner = w->objects()[1];
  m = inner->material();
  m.ambient = 1;
  inner->set_material(m);
  Ray r(Point(0, 0, 0.75f), Vector(0, 0, -1));
  ASSERT_EQ(inner->material().color, w->color_at(r));
}

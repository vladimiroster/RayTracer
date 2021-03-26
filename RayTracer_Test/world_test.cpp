#include "pch.h"
#include <Windows.h>

#include "../RayTracerLib/World.h"
#include "../RayTracerLib/Lighting.h"
#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Plane.h"

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
  auto comps = i.precompute(r, {i});
  auto c = w->shade_hit(comps);
  ASSERT_EQ(Color(0.38066f, 0.47583f, 0.2855f), c);
}

TEST(TestWorld, TestShadeIntersectionInside) {
  auto w = World::default_world();
  w->lights()[0] = std::make_shared<Light>(Color(1, 1, 1), Point(0, 0.25f, 0));
  Ray r(Point(0, 0, 0), Vector(0, 0, 1));
  auto shape = w->objects()[1];
  Intersection i(0.5, *shape);
  auto comps = i.precompute(r, {i});
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

TEST(TestWorld, TestNoShadowNothingColinearPointLight) {
  auto w = World::default_world();
  ASSERT_FALSE(w->is_shadowed(Point(0, 10, 0)));
}

TEST(TestWorld, TestShadowObjectBetweenPointLight) {
  auto w = World::default_world();
  ASSERT_TRUE(w->is_shadowed(Point(10, -10, 10)));
}

TEST(TestWorld, TestNoShadowObjectBehindLight) {
  auto w = World::default_world();
  ASSERT_FALSE(w->is_shadowed(Point(-20, 20, -20)));
}

TEST(TestWorld, TestNoShadowObjectBehindPoint) {
  auto w = World::default_world();
  ASSERT_FALSE(w->is_shadowed(Point(-2, 2, -2)));
}

TEST(TestWorld, TestIntersectionInShadow) {
  World w;
  w.lights().emplace_back(std::make_shared<Light>(Color(1, 1, 1), Point(0, 0, -10)));
  w.objects().emplace_back(std::make_shared<Sphere>());
  w.objects().emplace_back(std::make_shared<Sphere>(Transform::id().translate(0, 0, 10)));
  Ray r(Point(0, 0, 5), Vector(0, 0, 1));
  Intersection i(4, *w.objects()[1]);
  auto c = w.shade_hit(i.precompute(r, {i}));
  EXPECT_EQ(Color(0.1f, 0.1f, 0.1f), c);
}

TEST(TestWorld, TestNonreflectiveMaterial) {
  auto w = World::default_world();
  Ray r(Point(0, 0, 0), Vector(0, 0, 1));
  auto shape = w->objects()[1];
  auto m = shape->material();
  m.ambient = 1;
  shape->set_material(m);
  Intersection i(1.0f, *shape);
  auto comps = i.precompute(r, {i});
  auto color = w->reflected_color(comps, 1);
  EXPECT_EQ(black, color);
}

TEST(TestWorld, TestReflectiveMaterial) {
  auto w = World::default_world();
  Material m;
  m.reflective = 0.5;
  w->objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, -1, 0), m));
  Ray r(Point(0, 0, -3), Vector(0, -sqrtf(2)/2.0f, sqrtf(2)/2.0f));
  Intersection i(sqrtf(2), *w->objects().back());
  auto comps = i.precompute(r, {i});
  auto color = w->reflected_color(comps, 1);
  EXPECT_EQ(Color(0.19033f, 0.2379f, 0.14274f), color);
}

TEST(TestWorld, TestReflectiveShadeHit) {
  auto w = World::default_world();
  Material m;
  m.reflective = 0.5;
  w->objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, -1, 0), m));
  Ray r(Point(0, 0, -3), Vector(0, -sqrtf(2)/2.0f, sqrtf(2)/2.0f));
  Intersection i(sqrtf(2), *w->objects().back());
  auto comps = i.precompute(r, {i});
  auto color = w->shade_hit(comps);
  EXPECT_EQ(Color(0.87677f, 0.92436f, 0.82918f), color);
}

TEST(TestWorld, TestMutuallyReflective) {
  World w;
  w.lights().emplace_back(std::make_shared<Light>(Color(1, 1, 1), Point(0, 0, 0)));
  Material reflective_mat;
  reflective_mat.reflective = 1;
  w.objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, -1, 0), reflective_mat));
  w.objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, 1, 0), reflective_mat));
  Ray r(Point(0, 0, 0), Vector(0, 1, 0));
  try {
    w.color_at(r);
  }
  catch(...)
  {
    FAIL();
  }
}

TEST(TestWorld, TestReflectiveRecursionEnd) {
  auto w = World::default_world();
  Material m;
  m.reflective = 0.5;
  w->objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, -1, 0), m));
  Ray r(Point(0, 0, -3), Vector(0, -sqrtf(2)/2.0f, sqrtf(2)/2.0f));
  Intersection i(sqrtf(2), *w->objects().back());
  auto comps = i.precompute(r, {i});
  auto color = w->reflected_color(comps, 0);
  EXPECT_EQ(black, color);
}

TEST(TestWorld, TestRefractedColorOpaque) {
  auto w = World::default_world();
  auto shape = w->objects().front();
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  std::vector<Intersection> xs{Intersection(4, *shape), Intersection(6, *shape)};
  auto comps = xs[0].precompute(r, xs);
  auto c = w->refracted_color(comps, 5);
  EXPECT_EQ(black, c);
}

TEST(TestWorld, TestRefractedColorMaxRecursion) {
  auto w = World::default_world();
  Material m;
  m.transparency = 1;
  auto shape = w->objects().front();
  shape.get()->set_material(m);
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  std::vector<Intersection> xs{Intersection(4, *shape), Intersection(6, *shape)};
  auto comps = xs[0].precompute(r, xs);
  auto c = w->refracted_color(comps, 0);
  EXPECT_EQ(black, c);
}

TEST(TestWorld, TestRefractedColorTotalInternalReflection) {
  auto w = World::default_world();
  Material m;
  m.transparency = 1;
  m.refractive_index = 1.5f;
  auto shape = w->objects().front();
  shape.get()->set_material(m);
  Ray r(Point(0, 0, sqrtf(2)/2.0f), Vector(0, 1, 0));
  std::vector<Intersection> xs{Intersection(-sqrtf(2)/2.0f, *shape), Intersection(sqrtf(2)/2.0f, *shape)};
  // We're inside the sphere, so need to look at the second intersection
  auto comps = xs[1].precompute(r, xs);
  auto c = w->refracted_color(comps, 5);
  EXPECT_EQ(black, c);
}

TEST(TestWorld, TestRefractedColorHappyCase) {
  auto w = World::default_world();
  Material mA;
  mA.ambient = 1;
  mA.pattern = std::make_shared<TestPattern>();
  auto A = w->objects().front();
  A.get()->set_material(mA);
  Material mB;
  mB.transparency = 1;
  mB.refractive_index = 1.5;
  auto B = w->objects()[1];
  B.get()->set_material(mB);

  Ray r(Point(0, 0, 0.1f), Vector(0, 1, 0));
  std::vector<Intersection> xs{Intersection(-0.9899f, *A), Intersection(-0.4899f, *B), Intersection(0.4899f, *B), Intersection(0.9899f, *A)};
  auto comps = xs[2].precompute(r, xs);
  auto c = w->refracted_color(comps, 5);
  EXPECT_EQ(Color(0, 0.99888f, 0.04725f), c);
}

TEST(TestWorld, TestShadeHitTransparentMaterial) {
  auto w = World::default_world();
  Material floor_mat;
  floor_mat.transparency = 0.5;
  floor_mat.refractive_index = 1.5f;
  w->objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, -1, 0), floor_mat));
  Material ball_mat;
  ball_mat.ambient = 0.5f;
  ball_mat.color = Color(1, 0, 0);
  w->objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, -3.5f, -0.5f), ball_mat));

  Ray r(Point(0, 0, -3), Vector(0, -sqrtf(2)/2.0f, sqrtf(2)/2.0f));
  std::vector<Intersection> xs{Intersection(sqrtf(2), *w->objects()[2])};
  auto comps = xs[0].precompute(r, xs);
  auto color = w->shade_hit(comps, 5);
  EXPECT_EQ(Color(0.93642f, 0.68642f, 0.68642f), color);
}

TEST(TestWorld, TestShadeHitTransparentReflectiveMaterial) {
  auto w = World::default_world();
  Material floor_mat;
  floor_mat.reflective = 0.5f;
  floor_mat.transparency = 0.5;
  floor_mat.refractive_index = 1.5f;
  w->objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, -1, 0), floor_mat));
  Material ball_mat;
  ball_mat.ambient = 0.5f;
  ball_mat.color = Color(1, 0, 0);
  w->objects().emplace_back(std::make_shared<Plane>(Transform::id().translate(0, -3.5f, -0.5f), ball_mat));

  Ray r(Point(0, 0, -3), Vector(0, -sqrtf(2)/2.0f, sqrtf(2)/2.0f));
  std::vector<Intersection> xs{Intersection(sqrtf(2), *w->objects()[2])};
  auto comps = xs[0].precompute(r, xs);
  auto color = w->shade_hit(comps, 5);
  EXPECT_EQ(Color(0.93391f, 0.69643f, 0.69243f), color);
}

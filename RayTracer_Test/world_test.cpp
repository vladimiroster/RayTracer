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
#include "pch.h"

#include "../RayTracerLib/Tuple.h"
#include "../RayTracerLib/Materials.h"
#include "../RayTracerLib/Lighting.h"
#include "../RayTracerLib/Sphere.h"

using namespace RayTracer;

TEST(TestLighting, TestLightEyeSurface) {
  Material m;
  Point pos(0, 0, 0);

  Vector eyev(0, 0, -1);
  Vector normalv(0, 0, -1);
  Light light(Color(1, 1, 1), Point(0, 0, -10));

  Sphere s;

  auto result = lighting(m, std::ref(s), light, pos, eyev, normalv, false);
  EXPECT_EQ(Color(1.9f, 1.9f, 1.9f), result);
}

TEST(TestLighting, TestLightEyeSurfaceOffset45) {
  Material m;
  Point pos(0, 0, 0);

  Vector eyev(0, sqrtf(2)/2.0f, -sqrtf(2)/2.0f);
  Vector normalv(0, 0, -1);
  Light light(Color(1, 1, 1), Point(0, 0, -10));

  Sphere s;

  auto result = lighting(m, std::ref(s), light, pos, eyev, normalv, false);
  EXPECT_EQ(Color(1.0f, 1.0f, 1.0f), result);
}

TEST(TestLighting, TestEyeLightSurfaceOffset45) {
  Material m;
  Point pos(0, 0, 0);

  Vector eyev(0, 0, -1);
  Vector normalv(0, 0, -1);
  Light light(Color(1, 1, 1), Point(0, 10, -10));

  Sphere s;

  auto result = lighting(m, std::ref(s), light, pos, eyev, normalv, false);
  EXPECT_EQ(Color(0.7364f, 0.7364f, 0.7364f), result);
}

TEST(TestLighting, TestEyeOnReflection) {
  Material m;
  Point pos(0, 0, 0);

  Vector eyev(0, -sqrtf(2)/2.0f, -sqrtf(2)/2.0f);
  Vector normalv(0, 0, -1);
  Light light(Color(1, 1, 1), Point(0, 10, -10));

  Sphere s;

  auto result = lighting(m, std::ref(s), light, pos, eyev, normalv, false);
  EXPECT_EQ(Color(1.63638f, 1.63638f, 1.63638f), result);
}

TEST(TestLighting, TestEyeSurfaceLight) {
  Material m;
  Point pos(0, 0, 0);

  Vector eyev(0, 0, -1);
  Vector normalv(0, 0, -1);
  Light light(Color(1, 1, 1), Point(0, 0, 10));

  Sphere s;

  auto result = lighting(m, std::ref(s), light, pos, eyev, normalv, false);
  EXPECT_EQ(Color(0.1f, 0.1f, 0.1f), result);
}

TEST(TestLighting, TestSurfaceInShadow) {
  Material m;
  Point pos(0, 0, 0);

  Vector eyev(0, 0, -1);
  Vector normalv(0, 0, -1);
  Light light(Color(1, 1, 1), Point(0, 0, -10));

  Sphere s;

  auto result = lighting(m, std::ref(s), light, pos, eyev, normalv, true);
  EXPECT_EQ(Color(0.1f, 0.1f, 0.1f), result);
}

TEST(TestLighting, TestWithPattern) {
  Material m(black, 1, 0, 0, 0, 0, 0, 1, std::make_shared<StripePattern>(std::make_shared<SolidPattern>(white), std::make_shared<SolidPattern>(black)));
  Vector eyev(0, 0, -1);
  Vector normalv(0, 0, -1);
  Light l(Color(1, 1, 1), Point(0, 0, -10));
  Sphere s;
  EXPECT_EQ(Color(1, 1, 1), lighting(m, std::ref(s), l, Point(0.9f, 0, 0), eyev, normalv, false));
  EXPECT_EQ(Color(0, 0, 0), lighting(m, std::ref(s), l, Point(1.1f, 0, 0), eyev, normalv, false));
}

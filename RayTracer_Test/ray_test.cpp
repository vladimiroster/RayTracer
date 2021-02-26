#include "pch.h"
#include "../RayTracerLib/Ray.h"
#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Transformation.h"

using namespace RayTracer;

TEST(TestRay, TestRayCreate) {
  Point p(1, 2, 3);
  Vector v(4, 5, 6);
  Ray r(p, v);
  EXPECT_EQ(p, r.origin);
  EXPECT_EQ(v, r.direction);
}

TEST(TestRay, TestRayPosition) {
  Ray r(Point(2, 3, 4), Vector(1, 0, 0));
  EXPECT_EQ(Point(2, 3, 4), r.position(0));
  EXPECT_EQ(Point(3, 3, 4), r.position(1));
  EXPECT_EQ(Point(1, 3, 4), r.position(-1));
  EXPECT_EQ(Point(4.5, 3, 4), r.position(2.5));
}

TEST(TestRay, TestRayTranslate) {
  Ray r(Point(1, 2, 3), Vector(0, 1, 0));
  auto m = Transform::id().translate(3, 4, 5);
  auto r2 = m * r;
  EXPECT_EQ(Point(4, 6, 8), r2.origin);
  EXPECT_EQ(Vector(0, 1, 0), r2.direction);
}

TEST(TestRay, TestRayScale) {
  Ray r(Point(1, 2, 3), Vector(0, 1, 0));
  auto m = Transform::id().scale(2, 3, 4);
  auto r2 = m * r;
  EXPECT_EQ(Point(2, 6, 12), r2.origin);
  EXPECT_EQ(Vector(0, 3, 0), r2.direction);
}
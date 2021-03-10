#include "pch.h"

#include "../RayTracerLib/Plane.h"

using namespace RayTracer;

TEST(TestPlane, TestConstNormal) {
  Plane p;
  EXPECT_EQ(Vector(0, 1, 0), p.normal(Point(0, 0, 0)));
  EXPECT_EQ(Vector(0, 1, 0), p.normal(Point(10, 0, -10)));
  EXPECT_EQ(Vector(0, 1, 0), p.normal(Point(-5, 0, 150)));
}

TEST(TestPlane, TestParallelRayIntersect) {
  Plane p;
  Ray r(Point(0, 10, 0), Vector(0, 0, 1));
  auto xs = p.intersect(r);
  ASSERT_EQ(0, xs.size());
}

TEST(TestPlane, TestCoplanarRayIntersect) {
  Plane p;
  Ray r(Point(0, 0, 0), Vector(0, 0, 1));
  auto xs = p.intersect(r);
  ASSERT_EQ(0, xs.size());
}

TEST(TestPlane, TestRayAbove) {
  Plane p;
  Ray r(Point(0, 1, 0), Vector(0, -1, 0));
  auto xs = p.intersect(r);
  ASSERT_EQ(1, xs.size());
  EXPECT_EQ(1, xs[0].time());
  EXPECT_EQ(p, xs[0].object());
}

TEST(TestPlane, TestRayBelow) {
  Plane p;
  Ray r(Point(0, -1, 0), Vector(0, 1, 0));
  auto xs = p.intersect(r);
  ASSERT_EQ(1, xs.size());
  EXPECT_EQ(1, xs[0].time());
  EXPECT_EQ(p, xs[0].object());
}
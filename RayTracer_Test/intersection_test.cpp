#include "pch.h"

#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Intersection.h"

using namespace RayTracer;

TEST(TestIntersection, TestIntersectionCreate) {
  Sphere s;
  Intersection i(3.5f, s);

  EXPECT_EQ(3.5f, i.time());
  EXPECT_EQ(&s, &(i.object()));
}

TEST(TestIntersection, TestHitAllPositive) {
  Sphere s;
  Intersection i1(1, s);
  Intersection i2(2, s);
  std::vector<Intersection> xs{i2, i1};
  auto i = hit(xs);
  EXPECT_EQ(i1, i);
}

TEST(TestIntersection, TestHitMixed) {
  Sphere s;
  Intersection i1(-1, s);
  Intersection i2(1, s);
  std::vector<Intersection> xs{i2, i1};
  auto i = hit(xs);
  EXPECT_EQ(i2, i);
}

TEST(TestIntersection, TestHitAllNegative) {
  Sphere s;
  Intersection i1(-2, s);
  Intersection i2(-1, s);
  std::vector<Intersection> xs{i2, i1};
  auto i = hit(xs);
  EXPECT_FALSE(i.has_value());
}

TEST(TestIntersection, TestHitLowestNonNeg) {
  Sphere s;
  Intersection i1(5, s);
  Intersection i2(7, s);
  Intersection i3(-3, s);
  Intersection i4(2, s);
  std::vector<Intersection> xs{i1, i2, i3, i4};
  auto i = hit(xs);
  EXPECT_EQ(i4, i);
}

TEST(TestIntersection, TestPrecompute) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere s;
  Intersection i(4, s);
  auto comps = i.precompute(r);
  EXPECT_EQ(i.time(), comps.time);
  EXPECT_EQ(i.object(), comps.object);
  EXPECT_EQ(Point(0, 0, -1), comps.point);
  EXPECT_EQ(Vector(0, 0, -1), comps.eyev);
  EXPECT_EQ(Vector(0, 0, -1), comps.normalv);
}

TEST(TestIntersection, TestIntersectOutside) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere shape;
  Intersection i(4, shape);
  auto comps = i.precompute(r);
  ASSERT_EQ(false, comps.inside);
}

TEST(TestIntersection, TestIntersectInside) {
  Ray r(Point(0, 0, 0), Vector(0, 0, 1));
  Sphere shape;
  Intersection i(1, shape);
  auto comps = i.precompute(r);
  EXPECT_EQ(Point(0, 0, 1), comps.point);
  EXPECT_EQ(Vector(0, 0, -1), comps.eyev);
  EXPECT_EQ(true, comps.inside);
  // Normal inverted because the intersection is inside the object
  EXPECT_EQ(Vector(0, 0, -1), comps.normalv);
}

TEST(TestIntersection, TestHitOffsetsPoint) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere s(Transform::id().translate(0, 0, 1));
  Intersection i(5, s);
  auto comps = i.precompute(r);
  EXPECT_LE(comps.over_point.z, -epsilon / 2);
  EXPECT_GE(comps.point.z, comps.over_point.z);
}
#include "pch.h"

#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Intersection.h"

using namespace RayTracer;

TEST(TestIntersection, TestIntersectionCreate) {
  Sphere s;
  Intersection i(3.5f, s);

  EXPECT_EQ(3.5f, i.t);
  EXPECT_EQ(&s, &(i.s.get()));
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
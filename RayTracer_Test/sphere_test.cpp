#include "pch.h"
#include "../RayTracerLib/Ray.h"
#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Transformation.h"

using namespace RayTracer;

TEST(TestSphere, TestSphereRayIntersect) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere s;
  auto xs = s.intersect(r);
  EXPECT_EQ(2, xs.size());
  EXPECT_EQ(Intersection(4.0f, s), xs[0]);
  EXPECT_EQ(Intersection(6.0f, s), xs[1]);
}

TEST(TestSphere, TestSphereRayTangent) {
  Ray r(Point(0, 1, -5), Vector(0, 0, 1));
  Sphere s;
  auto xs = s.intersect(r);
  EXPECT_EQ(2, xs.size());
  EXPECT_EQ(Intersection(5.0f, s), xs[0]);
  EXPECT_EQ(Intersection(5.0f, s), xs[1]);
}

TEST(TestSphere, TestSphereRayMiss) {
  Ray r(Point(0, 2, -5), Vector(0, 0, 1));
  Sphere s;
  auto xs = s.intersect(r);
  EXPECT_EQ(0, xs.size());
}

TEST(TestSphere, TestSphereRayInside) {
  Ray r(Point(0, 0, 0), Vector(0, 0, 1));
  Sphere s;
  auto xs = s.intersect(r);
  EXPECT_EQ(2, xs.size());
  EXPECT_EQ(Intersection(-1.0f, s), xs[0]);
  EXPECT_EQ(Intersection(1.0f, s), xs[1]);
}

TEST(TestSphere, TestSphereRayBehind) {
  Ray r(Point(0, 0, 5), Vector(0, 0, 1));
  Sphere s;
  auto xs = s.intersect(r);
  EXPECT_EQ(2, xs.size());
  EXPECT_EQ(Intersection(-6.0f, s), xs[0]);
  EXPECT_EQ(Intersection(-4.0f, s), xs[1]);
}

TEST(TestSphere, TestNormalX) {
  Sphere s;
  auto n = s.normal(Point(1, 0, 0));
  EXPECT_EQ(Vector(1, 0, 0), n);
}

TEST(TestSphere, TestNormalY) {
  Sphere s;
  auto n = s.normal(Point(0, 1, 0));
  EXPECT_EQ(Vector(0, 1, 0), n);
}

TEST(TestSphere, TestNormalZ) {
  Sphere s;
  auto n = s.normal(Point(0, 0, 1));
  EXPECT_EQ(Vector(0, 0, 1), n);
}

TEST(TestSphere, TestNormalNonaxial) {
  Sphere s;
  auto n = s.normal(Point(sqrtf(3)/3.0f, sqrtf(3)/3.0f, sqrtf(3)/3.0f));
  EXPECT_EQ(Vector(sqrtf(3)/3.0f, sqrtf(3)/3.0f, sqrtf(3)/3.0f), n);
}

TEST(TestSphere, TestNormalNormalized) {
  Sphere s;
  auto n = s.normal(Point(sqrtf(3)/3.0f, sqrtf(3)/3.0f, sqrtf(3)/3.0f));
  EXPECT_EQ(normalize(n), n);
}

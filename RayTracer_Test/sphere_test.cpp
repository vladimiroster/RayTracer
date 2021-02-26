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

TEST(TestSphere, TestSphereDefaultTransform) {
  Sphere s;
  EXPECT_EQ(Transform::id(), s.transform());
}

TEST(TestSphere, TestSphereCustomTransform) {
  Sphere s(Transform::id().translate(2, 3, 4));
  EXPECT_EQ(Transform::id().translate(2, 3, 4), s.transform());
  EXPECT_EQ(Transform::id().translate(2, 3, 4).inverse(), s.inverse());
}

TEST(TestSphere, TestScaledSphereIntersect) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere s(Transform::id().scale(2, 2, 2));

  auto xs = s.intersect(r);
  EXPECT_EQ(2, xs.size());
  EXPECT_TRUE(ApproxEqual(3, xs[0].time(), epsilon));
  EXPECT_TRUE(ApproxEqual(7, xs[1].time(), epsilon));
}

TEST(TestSphere, TestTranslatedSphereIntersect) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere s(Transform::id().translate(5, 0, 0));

  auto xs = s.intersect(r);
  EXPECT_EQ(0, xs.size());
}
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

TEST(TestSphere, TestTranslatedNormal) {
  Sphere s(Transform::id().translate(0, 1, 0));
  auto n = s.normal(Point(0, 1.70711f, -0.70711f));
  EXPECT_EQ(Vector(0, 0.70711f, -0.70711f), n);
}

TEST(TestSphere, TestTransformedNormal) {
  Sphere s(Transform::id().rot_z(M_PI / 5.0f).scale(1, 0.5f, 1));
  auto n = s.normal(Point(0, sqrtf(2)/2, -sqrtf(2)/2));
  EXPECT_EQ(Vector(0, 0.97014f, -0.24254f), n);
}

TEST(TestSphere, TestDefaultMaterial) {
  Sphere s;
  EXPECT_EQ(Material(), s.material());
}

TEST(TestSphere, TestCustomMaterial) {
  Sphere s;
  Material m;
  m.ambient = 1;
  s.set_material(m);
  EXPECT_EQ(m, s.material());
}
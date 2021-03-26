#include "pch.h"

#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Plane.h"
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
  auto comps = i.precompute(r, {i});
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
  auto comps = i.precompute(r, {i});
  ASSERT_EQ(false, comps.inside);
}

TEST(TestIntersection, TestIntersectInside) {
  Ray r(Point(0, 0, 0), Vector(0, 0, 1));
  Sphere shape;
  Intersection i(1, shape);
  auto comps = i.precompute(r, {i});
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
  auto comps = i.precompute(r, {i});
  EXPECT_LE(comps.over_point.z, -epsilon / 2);
  EXPECT_GE(comps.point.z, comps.over_point.z);
}

TEST(TestIntersection, TestPrecomputeReflection) {
  Plane shape;
  Ray r(Point(0, 1, -1), Vector(0, -sqrtf(2)/2.0f, sqrtf(2)/2.0f));
  Intersection i(sqrtf(2), shape);
  auto comps = i.precompute(r, {i});
  EXPECT_EQ(Vector(0, sqrtf(2)/2.0f, sqrtf(2)/2.0f), comps.reflectv);
}

TEST(TestIntersection, TestRefractiveIndexCompute) {
  Sphere A(Transform::id().scale(2, 2, 2), Material(white, 0.1f, 0.9f, 0.9f, 200, 0.0f, 1.0f, 1.5f));
  Sphere B(Transform::id().translate(0, 0, -0.25), Material(white, 0.1f, 0.9f, 0.9f, 200, 0.0f, 1.0f, 2));
  Sphere C(Transform::id().translate(0, 0, 0.25), Material(white, 0.1f, 0.9f, 0.9f, 200, 0.0f, 1.0f, 2.5f));
  Ray r(Point(0, 0, -4), Vector(0, 0, 1));
  std::vector<Intersection> xs{
    Intersection(2, std::ref(A)), 
    Intersection(2.75f, std::ref(B)), 
    Intersection(3.25f, std::ref(C)), 
    Intersection(4.75f, std::ref(B)),
    Intersection(5.25f, std::ref(C)),
    Intersection(6, std::ref(A))};

  std::array<float, 6> ref_n1{1, 1.5f, 2, 2.5f, 2.5f, 1.5f};
  std::array<float, 6> ref_n2{1.5f, 2, 2.5f, 2.5f, 1.5f, 1};

  for (size_t i = 0; i < xs.size(); ++i) {
    auto comps = xs[i].precompute(r, xs);
    EXPECT_EQ(ref_n1[i], comps.n1);
    EXPECT_EQ(ref_n2[i], comps.n2);
  }
}

TEST(TestIntersection, TestPrecomputeUnderPoint) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere shape(Transform::id().translate(0, 0, 1), Material(white, 0.1f, 0.9f, 0.9f, 200, 0.0f, 1.0f, 1.5f));
  Intersection i(5, shape);
  std::vector<Intersection> xs{i};
  auto comps = i.precompute(r, xs);
  EXPECT_TRUE(comps.under_point.z > epsilon / 2.0f);
  EXPECT_TRUE(comps.point.z < comps.under_point.z);
}

TEST(TestIntersection, TestSchlickTotalInternalReflection) {
  Material glass_mat;
  glass_mat.transparency = 1;
  glass_mat.refractive_index = 1.5f;
  Sphere shape(Transform::id(), glass_mat);
  Ray r(Point(0, 0, sqrtf(2)/2.0f), Vector(0, 1, 0));
  std::vector<Intersection> xs{Intersection(-sqrtf(2)/2.0f, shape), Intersection(sqrtf(2)/2.0f, shape)};
  auto comps = xs[1].precompute(r, xs);
  auto reflectance = schlick(comps);
  EXPECT_TRUE(ApproxEqual(1.0f, reflectance, epsilon));
}

TEST(TestIntersection, TestSchlickPerpendicularRay) {
  Material glass_mat;
  glass_mat.transparency = 1;
  glass_mat.refractive_index = 1.5f;
  Sphere shape(Transform::id(), glass_mat);
  Ray r(Point(0, 0, 0), Vector(0, 1, 0));
  std::vector<Intersection> xs{Intersection(-1, shape), Intersection(1, shape)};
  auto comps = xs[1].precompute(r, xs);
  auto reflectance = schlick(comps);
  EXPECT_TRUE(ApproxEqual(0.04f, reflectance, epsilon));
}

TEST(TestIntersection, TestSchlickSmallAngle) {
  Material glass_mat;
  glass_mat.transparency = 1;
  glass_mat.refractive_index = 1.5f;
  Sphere shape(Transform::id(), glass_mat);
  Ray r(Point(0, 0.99f, -2), Vector(0, 0, 1));
  std::vector<Intersection> xs{Intersection(1.8589, shape)};
  auto comps = xs[0].precompute(r, xs);
  auto reflectance = schlick(comps);
  EXPECT_TRUE(ApproxEqual(0.48873f, reflectance, epsilon));
}

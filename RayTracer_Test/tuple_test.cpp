#include "pch.h"
#include "../RayTracerLib/Tuple.h"

using namespace RayTracer;

TEST(TestTuple, TestTuplePointCreate) {
  Tuple a(4.3f, -4.2f, 3.1f, 1.0f);

  EXPECT_TRUE(ApproxEqual(a.x, 4.3f, epsilon));
  EXPECT_TRUE(ApproxEqual(a.y, -4.2f, epsilon));
  EXPECT_TRUE(ApproxEqual(a.z, 3.1f, epsilon));
  EXPECT_TRUE(ApproxEqual(a.w, 1.0f, epsilon));
}

TEST(TestTuple, TestTupleVectorCreate) {
  Tuple a(4.3f, -4.2f, 3.1f, 0.0f);

  EXPECT_TRUE(ApproxEqual(a.x, 4.3f, epsilon));
  EXPECT_TRUE(ApproxEqual(a.y, -4.2f, epsilon));
  EXPECT_TRUE(ApproxEqual(a.z, 3.1f, epsilon));
  EXPECT_TRUE(ApproxEqual(a.w, 0.0f, epsilon));
}
TEST(TestTuple, TestPointCreate) {
  Point p(4.0f, -4.0f, 3.0f);

  EXPECT_EQ(Tuple(4.0f, -4.0f, 3.0f, 1.0f), p);
}

TEST(TestTuple, TestVectorCreate) {
  Vector p(4.0f, -4.0f, 3.0f);

  EXPECT_EQ(Tuple(4.0f, -4.0f, 3.0f, 0.0f), p);
}

TEST(TestTuple, TestTupleAdd) {
  Tuple a(3.0f, -2.0f, 5.0f, 1.0f);
  Tuple b(-2.0f, 3.0f, 1.0f, 0.0f);

  EXPECT_EQ(Tuple(1.0f, 1.0f, 6.0f, 1.0f), a+b);
}

TEST(TestTuple, TestPointPointSub) {
  Point a(3.0f, 2.0f, 1.0f);
  Point b(5.0f, 6.0f, 7.0f);

  EXPECT_EQ(Vector(-2.0f, -4.0f, -6.0f), a-b);
}

TEST(TestTuple, TestPointVectorSub) {
  Point  a(3.0f, 2.0f, 1.0f);
  Vector b(5.0f, 6.0f, 7.0f);

  EXPECT_EQ(Point(-2.0f, -4.0f, -6.0f), a-b);
}

TEST(TestTuple, TestVectorVectorSub) {
  Vector a(3.0f, 2.0f, 1.0f);
  Vector b(5.0f, 6.0f, 7.0f);

  EXPECT_EQ(Vector(-2.0f, -4.0f, -6.0f), a-b);
}

TEST(TestTuple, TestTupleNeg) {
  Tuple a(1.0f, -2.0f, 3.0f, -4.0f);

  EXPECT_EQ(Tuple(-1.0f, 2.0f, -3.0f, 4.0f), -a);
}

TEST(TestTuple, TestMulTupleScalar) {
  Tuple a(1.0f, -2.0f, 3.0f, -4.0f);
  EXPECT_EQ(Tuple(3.5f, -7.0, 10.5f, -14.0f), a * 3.5f);
}

TEST(TestTuple, TestMulTupleFraction) {
  Tuple a(1.0f, -2.0f, 3.0f, -4.0f);
  EXPECT_EQ(Tuple(0.5f, -1.0, 1.5f, -2.0f), a * 0.5f);
}

TEST(TestTuple, TestDivTupleScalar) {
  Tuple a(1.0f, -2.0f, 3.0f, -4.0f);
  EXPECT_EQ(Tuple(0.5f, -1.0, 1.5f, -2.0f), a / 2.0f);
}

TEST(TestTuple, TestMagVec100) {
  Vector v(1.0f, 0.0f, 0.0f);
  EXPECT_EQ(1.0f, magnitude(v));
}

TEST(TestTuple, TestMagVec010) {
  Vector v(0.0f, 1.0f, 0.0f);
  EXPECT_EQ(1.0f, magnitude(v));
}

TEST(TestTuple, TestMagVec001) {
  Vector v(0.0f, 0.0f, 1.0f);
  EXPECT_EQ(1.0f, magnitude(v));
}

TEST(TestTuple, TestMagVec) {
  Vector v(1.0f, 2.0f, 3.0f);
  EXPECT_EQ(sqrtf(14.0f), magnitude(v));
}

TEST(TestTuple, TestMagVecNeg) {
  Vector v(-1.0f, -2.0f, -3.0f);
  EXPECT_EQ(sqrtf(14.0f), magnitude(v));
}

TEST(TestTuple, TestNormalizeVecSingle) {
  Vector v(4.0f, 0.0f, 0.0f);
  EXPECT_EQ(Vector(1.0f, 0.0f, 0.0f), normalize(v));
}

TEST(TestTuple, TestNormalizeVecRegular) {
  Vector v(1.0f, 2.0f, 3.0f);
  EXPECT_EQ(Vector(0.26726f, 0.53452f, 0.80178f), normalize(v));
}

TEST(TestTuple, TestVectorDotProduct) {
  Vector v1(1.0f, 2.0f, 3.0f);
  Vector v2(2.0f, 3.0f, 4.0f);

  EXPECT_EQ(20.0f, dot(v1, v2));
}

TEST(TestTuple, TestVectorCrossProduct) {
  Vector v1(1.0f, 2.0f, 3.0f);
  Vector v2(2.0f, 3.0f, 4.0f);

  EXPECT_EQ(Vector(-1.0f, 2.0f, -1.0f), cross(v1, v2));
  EXPECT_EQ(Vector(1.0f, -2.0f, 1.0f), cross(v2, v1));
}

TEST(TestTuple, TestColorCreate) {
  Color c(-0.5f, 0.4f, 1.7f);
  EXPECT_EQ(Tuple(-0.5f, 0.4f, 1.7f, 0.0f), c);
}

TEST(TestTuple, TestColorAdd) {
  Color c1(0.9f, 0.6f, 0.75f);
  Color c2(0.7f, 0.1f, 0.25f);
  EXPECT_EQ(Color(1.6f, 0.7f, 1.0f), c1 + c2);
}

TEST(TestTuple, TestColorSub) {
  Color c1(0.9f, 0.6f, 0.75f);
  Color c2(0.7f, 0.1f, 0.25f);
  EXPECT_EQ(Color(0.2f, 0.5f, 0.5f), c1 - c2);
}

TEST(TestTuple, TestColorMulScalar) {
  Color c1(0.3f, 0.4f, 0.5f);
  EXPECT_EQ(Color(0.6f, 0.8f, 1.0f), c1 * 2);
}

TEST(TestTuple, TestColorMulColor) {
  Color c1(1.0f, 0.2f, 0.4f);
  Color c2(0.9f, 1.0f, 0.1f);
  EXPECT_EQ(Color(0.9f, 0.2f, 0.04f), c1 * c2);
}

TEST(TestTuple, TestReflect45) {
  Vector v(1, -1, 0);
  Vector n(0, 1, 0);
  auto r = reflect(v, n);
  EXPECT_EQ(Vector(1, 1, 0), r);
}

TEST(TestTuple, TestReflectSlanted) {
  Vector v(0, -1, 0);
  Vector n(sqrtf(2)/2.0f, sqrtf(2)/2.0f, 0);
  auto r = reflect(v, n);
  EXPECT_EQ(Vector(1, 0, 0), r);
}

TEST(TestTuple, TestCarthesianSpherical) {
  Vector v(3, 4, 5);
  auto sph = spherical(v);
  auto cart = carthesian(sph);
  ASSERT_EQ(v, cart);
}
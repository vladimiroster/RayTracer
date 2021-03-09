#include "pch.h"
#include "../RayTracerLib/Transformation.h"

using namespace RayTracer;

TEST(TestTransformation, TestTranslation) {
  auto transform = Transform::id().translate(5, -3, 2);
  Point p(-3, 4, 5);
  ASSERT_EQ(Point(2, 1, 7), transform * p);
}

TEST(TestTransformation, TestTranslationReverse) {
  auto transform = Transform::id().translate(5, -3, 2);
  auto inv = transform.inverse();
  Point p(-3, 4, 5);
  ASSERT_EQ(Point(-8, 7, 3), inv * p);
}

TEST(TestTransformation, TestTranslationVectorNoChange) {
  auto transform = Transform::id().translate(5, -3, 2);
  Vector v(-3, 4, 5);
  ASSERT_EQ(v, transform * v);
}

TEST(TestTransformation, TestScalePoint) {
  auto transform = Transform::id().scale(2, 3, 4);
  Point p(-4, 6, 8);
  ASSERT_EQ(Point(-8, 18, 32), transform * p);
}

TEST(TestTransformation, TestScaleVector) {
  auto transform = Transform::id().scale(2, 3, 4);
  Vector v(-4, 6, 8);
  ASSERT_EQ(Vector(-8, 18, 32), transform * v);
}

TEST(TestTransformation, TestInverseScaleVector) {
  auto transform = Transform::id().scale(2, 3, 4);
  auto inv = transform.inverse();
  Vector v(-4, 6, 8);
  ASSERT_EQ(Vector(-2, 2, 2), inv * v);
}

TEST(TestTransformation, TestReflection) {
  auto transform = Transform::id().scale(-1, 1, 1);
  Point p(2, 3, 4);
  ASSERT_EQ(Point(-2, 3, 4), transform * p);
}

TEST(TestTransformation, TestRotX) {
  Point p(0, 1, 0);
  auto half_quarter = Transform::id().rot_x(M_PI / 4);
  auto full_quarter = Transform::id().rot_x(M_PI / 2);

  EXPECT_EQ(Point(0, std::sqrtf(2)/2.0f, std::sqrtf(2)/2.0f), half_quarter * p);
  EXPECT_EQ(Point(0, 0, 1), full_quarter * p);
}

TEST(TestTransformation, TestRotXInv) {
  Point p(0, 1, 0);
  auto half_quarter = Transform::id().rot_x(M_PI / 4);

  EXPECT_EQ(Point(0, std::sqrtf(2)/2.0f, -std::sqrtf(2)/2.0f), half_quarter.inverse() * p);
}

TEST(TestTransformation, TestRotY) {
  Point p(0, 0, 1);
  auto half_quarter = Transform::id().rot_y(M_PI / 4);
  auto full_quarter = Transform::id().rot_y(M_PI / 2);

  EXPECT_EQ(Point(std::sqrtf(2)/2.0f, 0, std::sqrtf(2)/2.0f), half_quarter * p);
  EXPECT_EQ(Point(1, 0, 0), full_quarter * p);
}

TEST(TestTransformation, TestRotZ) {
  Point p(0, 1, 0);
  auto half_quarter = Transform::id().rot_z(M_PI / 4);
  auto full_quarter = Transform::id().rot_z(M_PI / 2);

  EXPECT_EQ(Point(-std::sqrtf(2)/2.0f, std::sqrtf(2)/2.0f, 0), half_quarter * p);
  EXPECT_EQ(Point(-1, 0, 0), full_quarter * p);
}

TEST(TestTransformation, TestShearXY) {
  auto t = Transform::id().shear(1, 0, 0, 0, 0, 0);
  Point p(2, 3, 4);

  ASSERT_EQ(Point(5, 3, 4), t * p);
}

TEST(TestTransformation, TestShearXZ) {
  auto t = Transform::id().shear(0, 1, 0, 0, 0, 0);
  Point p(2, 3, 4);

  ASSERT_EQ(Point(6, 3, 4), t * p);
}

TEST(TestTransformation, TestShearYX) {
  auto t = Transform::id().shear(0, 0, 1, 0, 0, 0);
  Point p(2, 3, 4);

  ASSERT_EQ(Point(2, 5, 4), t * p);
}

TEST(TestTransformation, TestShearYZ) {
  auto t = Transform::id().shear(0, 0, 0, 1, 0, 0);
  Point p(2, 3, 4);

  ASSERT_EQ(Point(2, 7, 4), t * p);
}

TEST(TestTransformation, TestShearZX) {
  auto t = Transform::id().shear(0, 0, 0, 0, 1, 0);
  Point p(2, 3, 4);

  ASSERT_EQ(Point(2, 3, 6), t * p);
}

TEST(TestTransformation, TestShearZY) {
  auto t = Transform::id().shear(0, 0, 0, 0, 0, 1);
  Point p(2, 3, 4);

  ASSERT_EQ(Point(2, 3, 7), t * p);
}

TEST(TestTransformation, TestTransformSequence) {
  Point p(1, 0, 1);
  auto A = Transform::id().rot_x(M_PI / 2.0f);
  auto B = Transform::id().scale(5, 5, 5);
  auto C = Transform::id().translate(10, 5, 7);

  auto p2 =  A * p;
  EXPECT_EQ(Point(1, -1, 0), p2);
  auto p3 = B * p2;
  EXPECT_EQ(Point(5, -5, 0), p3);
  auto p4 = C * p3;
  EXPECT_EQ(Point(15, 0, 7), p4);
}

TEST(TestTransformation, TestTransformChain) {
  Point p(1, 0, 1);
  auto T = Transform::id().rot_x(M_PI / 2.0f).scale(5, 5, 5).translate(10, 5, 7);
  EXPECT_EQ(Point(15, 0, 7), T * p);
}

TEST(TestTransformation, TestViewDefault) {
  Point from(0, 0, 0), to(0, 0, -1);
  Vector up(0, 1, 0);
  auto t = Transform::id().view(from, to, up);
  ASSERT_EQ(Transform::id(), t);
}

TEST(TestTransformation, TestViewPosZ) {
  Point from(0, 0, 0), to(0, 0, 1);
  Vector up(0, 1, 0);
  auto t = Transform::id().view(from, to, up);
  ASSERT_EQ(Transform::id().scale(-1, 1, -1), t);
}

TEST(TestTransformation, TestViewMovesWorld) {
  Point from(0, 0, 8), to(0, 0, 0);
  Vector up(0, 1, 0);
  auto t = Transform::id().view(from, to, up);
  ASSERT_EQ(Transform::id().translate(0, 0, -8), t);
}

TEST(TestTransformation, TestViewArbitrary) {
  Point from(1, 3, 2), to(4, -2, 8);
  Vector up(1, 1, 0);
  auto t = Transform::id().view(from, to, up);
  ASSERT_EQ(Matrix<4>({
    -0.50709f, 0.50709f, 0.67612f, -2.36643f,
    0.76772f, 0.60609f, 0.12122f, -2.82843f,
    -0.35857f, 0.59761f, -0.71714f, 0,
    0, 0, 0, 1}), t);
}


#include "pch.h"
#include "../RayTracerLib/Matrix.h"

using namespace RayTracer;

TEST(TestMatrix, TestMatrix2x2Create) {
  Matrix<2> m({-3, 5,
    1, -2});

  EXPECT_TRUE(ApproxEqual(m[0][0], -3.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[0][1], 5.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][0], 1.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][1], -2.0f, epsilon));
}

TEST(TestMatrix, TestMatrix3x3Create) {
  Matrix<3> m({-3, 5, 0,
    1, -2, -7,
    0, 1, 1});

  EXPECT_TRUE(ApproxEqual(m[0][0], -3.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[0][1], 5.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[0][2], 0.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][0], 1.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][1], -2.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][2], -7.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[2][0], 0.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[2][1], 1.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[2][2], 1.0f, epsilon));
}

TEST(TestMatrix, TestMatrix4x4Create) {
  Matrix<4> m({1, 2, 3, 4,
    5.5f, 6.5f, 7.5f, 8.5f,
    9, 10, 11, 12,
    13.5f, 14.5f, 15.5f, 16.5f});

  EXPECT_TRUE(ApproxEqual(m[0][0], 1.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[0][1], 2.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[0][2], 3.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[0][3], 4.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][0], 5.5f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][1], 6.5f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][2], 7.5f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[1][3], 8.5f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[2][0], 9.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[2][1], 10.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[2][2], 11.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[2][3], 12.0f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[3][0], 13.5f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[3][1], 14.5f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[3][2], 15.5f, epsilon));
  EXPECT_TRUE(ApproxEqual(m[3][3], 16.5f, epsilon));
}

TEST(TestMatrix, TestMatrix2x2EqIdentical) {
  Matrix<2> m1({1, 2, 
    3, 4});
  Matrix<2> m2({1, 2, 
    3, 4});

  EXPECT_EQ(m1, m2);
}

TEST(TestMatrix, TestMatrix2x2EqDiff) {
  Matrix<2> m1({1, 2, 
    3, 4});
  Matrix<2> m2({2, 3, 
    4, 5});

  EXPECT_NE(m1, m2);
}

TEST(TestMatrix, TestMatrix3x3EqIdentical) {
  Matrix<3> m1({1, 2, 3, 4, 5, 6, 7, 8, 9});
  Matrix<3> m2({1, 2, 3, 4, 5, 6, 7, 8, 9});

  EXPECT_EQ(m1, m2);
}

TEST(TestMatrix, TestMatrix3x3EqDiff) {
  Matrix<3> m1({1, 2, 3, 4, 5, 6, 7, 8, 9});
  Matrix<3> m2({2, 3, 4, 5, 6, 7, 8, 9, 10});

  EXPECT_NE(m1, m2);
}

TEST(TestMatrix, TestMatrix4x4EqIdentical) {
  Matrix<4> m1({1, 2, 3, 4,
    5, 6, 7, 8,
    9, 8, 7, 6,
    5, 4, 3, 2});
  Matrix<4> m2({1, 2, 3, 4,
    5, 6, 7, 8,
    9, 8, 7, 6,
    5, 4, 3, 2});

  EXPECT_EQ(m1, m2);
}

TEST(TestMatrix, TestMatrix4x4EqDiff) {
  Matrix<4> m1({1, 2, 3, 4,
    5, 6, 7, 8,
    9, 8, 7, 6,
    5, 4, 3, 2});
  Matrix<4> m2({2, 3, 4, 5,
    6, 7, 8, 9,
    8, 7, 6, 5,
    4, 3, 2, 1});

  EXPECT_NE(m1, m2);
}

TEST(TestMatrix, TestMatrix4x4Mul) {
  Matrix<4> m1({1, 2, 3, 4,
    5, 6, 7, 8,
    9, 8, 7, 6,
    5, 4, 3, 2});
  Matrix<4> m2({-2, 1, 2, 3,
    3, 2, 1, -1,
    4, 3, 6, 5,
    1, 2, 7, 8});

  EXPECT_EQ(Matrix<4>({20, 22, 50, 48,
    44, 54, 114, 108,
    40, 58, 110, 102,
    16, 26, 46, 42}), m1 * m2);
}

TEST(TestMatrix, TestMatrix4x4MulTuple) {
  Matrix<4> m({1, 2, 3, 4,
    2, 4, 4, 2,
    8, 6, 4, 1,
    0, 0, 0, 1});
  Tuple t(1, 2, 3, 1);

  EXPECT_EQ(Tuple(18, 24, 33, 1), m * t);
}

TEST(TestMatrix, TestMatrix4x4MulId) {
  Matrix<4> m({0, 1, 2, 4,
    1, 2, 4, 8,
    2, 4, 8, 16,
    4, 8, 16, 32});

  EXPECT_EQ(m, m * id4);
}

TEST(TestMatrix, TestMatrixId4MulTuple) {
  Tuple t(1, 2, 3, 4);

  EXPECT_EQ(t, id4 * t);
}

TEST(TestMatrix, TestMatrix4x4Transpose) {
  Matrix<4> m({0, 9, 3, 0,
    9 ,8 ,0 ,8,
    1, 8, 5, 3,
    0 ,0 ,5 ,8});

  EXPECT_EQ(Matrix<4>({0, 9, 1, 0,
    9 ,8 ,8 ,0,
    3, 0, 5, 5,
    0, 8, 3, 8}), m.transpose());
}

TEST(TestMatrix, TestId4Transpose) {
  EXPECT_EQ(id4, id4.transpose());
}

TEST(TestMatrix, TestDet2) {
  Matrix<2> m({1, 5, 
    -3, 2});

  EXPECT_TRUE(ApproxEqual(17, m.det(), epsilon));
}

TEST(TestMatrix, Test3x3SubMatrix) {
  Matrix<3> m({1, 5, 0,
    -3, 2, 7,
    0, 6, -3});

  EXPECT_EQ(Matrix<2>({-3, 2, 0, 6}), m.submatrix(0, 2));
}

TEST(TestMatrix, Test4x4SubMatrix) {
  Matrix<4> m({-6, 1, 1, 6,
    -8, 5, 8, 6,
    -1, 0, 8, 2,
    -7, 1, -1, 1});

  EXPECT_EQ(Matrix<3>({-6, 1, 6,
    -8, 8, 6,
    -7, -1, 1}), m.submatrix(2, 1));
}

TEST(TestMatrix, Test3x3Minor) {
  Matrix<3> m({3, 5, 0,
    2, -1, -7,
    6, -1, 5});

  Matrix<2> m2 = m.submatrix(1, 0);

  EXPECT_TRUE(ApproxEqual(25.0f, m2.det(), epsilon));
  EXPECT_TRUE(ApproxEqual(25.0f, m.minor(1, 0), epsilon));
}

TEST(TestMatrix, Test3x3Cofactor) {
  Matrix<3> m({3, 5, 0,
    2, -1, -7,
    6, -1, 5});

  EXPECT_TRUE(ApproxEqual(-12, m.minor(0, 0), epsilon));
  EXPECT_TRUE(ApproxEqual(-12, m.cofactor(0, 0), epsilon));
  EXPECT_TRUE(ApproxEqual(25, m.minor(1, 0), epsilon));
  EXPECT_TRUE(ApproxEqual(-25, m.cofactor(1, 0), epsilon));
}

TEST(TestMatrix, TestDetMat3) {
  Matrix<3> m({1, 2, 6,
    -5, 8, -4,
    2, 6, 4});

  EXPECT_TRUE(ApproxEqual(56, m.cofactor(0, 0), epsilon));
  EXPECT_TRUE(ApproxEqual(12, m.cofactor(0, 1), epsilon));
  EXPECT_TRUE(ApproxEqual(-46, m.cofactor(0, 2), epsilon));
  EXPECT_TRUE(ApproxEqual(-196, m.det(), epsilon));
}

TEST(TestMatrix, TestDetMat4) {
  Matrix<4> m({-2, -8, 3, 5,
    -3, 1, 7, 3,
    1, 2, -9, 6,
    -6, 7, 7, -9});

  EXPECT_TRUE(ApproxEqual(690, m.cofactor(0, 0), epsilon));
  EXPECT_TRUE(ApproxEqual(447, m.cofactor(0, 1), epsilon));
  EXPECT_TRUE(ApproxEqual(210, m.cofactor(0, 2), epsilon));
  EXPECT_TRUE(ApproxEqual(51, m.cofactor(0, 3), epsilon));
  EXPECT_TRUE(ApproxEqual(-4071, m.det(), epsilon));
}

TEST(TestMatrix, TestDet0) {
  Matrix<4> m({-4, 2, -2, -3,
    9, 6, 2, 6,
    0, -5, 1, -5,
    0, 0, 0, 0});

  EXPECT_TRUE(ApproxEqual(0.0f, m.det(), epsilon));
}

TEST(TestMatrix, TestInverse) {
  Matrix<4> a({-5, 2, 6, -8,
    1, -5, 1, 8,
    7, 7, -6, -7,
    1, -3, 7, 4});

  auto b = a.inverse();

  EXPECT_TRUE(ApproxEqual(532, a.det(), epsilon));
  EXPECT_TRUE(ApproxEqual(-160, a.cofactor(2, 3), epsilon));
  EXPECT_TRUE(ApproxEqual(-160.0f/532.0f, b[3][2], epsilon));
  EXPECT_TRUE(ApproxEqual(105, a.cofactor(3, 2), epsilon));
  EXPECT_TRUE(ApproxEqual(105.0f/532.0f, b[2][3], epsilon));
  EXPECT_EQ(Matrix<4>({0.21805f, 0.45113f, 0.24060f, -0.04511f,
    -0.80827f, -1.45677f, -0.44361f, 0.52068f,
    -0.07895f, -0.22368f, -0.05263f, 0.19737f,
    -0.52256f, -0.81391f, -0.30075f, 0.30639f}), b);
}

TEST(TestMatrix, TestInverse2) {
  Matrix<4> a({8, -5, 9, 2,
    7, 5, 6, 1,
    -6, 0, 9, 6,
    -3, 0, -9, -4});

  EXPECT_EQ(Matrix<4>({-0.15385f, -0.15385f, -0.28205f, -0.53846f,
    -0.07692f, 0.12308f, 0.02564f, 0.03077f,
    0.35897f, 0.35897f, 0.43590f, 0.92308f,
    -0.69231f, -0.69231f, -0.76923f, -1.92308f}), a.inverse());
}

TEST(TestMatrix, TestInverse3) {
  Matrix<4> a({9, 3, 0, 9,
    -5, -2, -6, -3,
    -4, 9, 6, 4,
    -7, 6, 6, 2});

  EXPECT_EQ(Matrix<4>({-0.04074f, -0.07778f, 0.14444f, -0.22222f,
    -0.07778f, 0.03333f, 0.36667f, -0.33333f,
    -0.02901f, -0.14630f, -0.10926f, 0.12963f,
    0.17778f, 0.06667f, -0.26667f, 0.33333f}), a.inverse());
}

TEST(TestMatrix, TestInverseMultiply) {
  Matrix<4> a({3, -9, 7, 3,
    3, -8, 2, -9,
    -4, 4, 4, 1,
    -6, 5, -1, 1});
  Matrix<4> b({8, 2, 2, 2,
    3, -1, 7, 0,
    7, 0, 5, 4,
    6, -2, 0, 5});

  auto c = a * b;
  EXPECT_EQ(a, c * b.inverse());
}

TEST(TestMatrix, TestIdInverse) {
  EXPECT_EQ(id4, id4.inverse());
}

TEST(TestMatrix, TestMatInverseID) {
  Matrix<4> a({9, 3, 0, 9,
    -5, -2, -6, -3,
    -4, 9, 6, 4,
    -7, 6, 6, 2});

  EXPECT_EQ(id4, a * a.inverse());
}
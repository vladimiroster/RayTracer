#include "pch.h"

#include "../RayTracerLib/Pattern.h"
#include "../RayTracerLib/Tuple.h"
#include "../RayTracerLib/Sphere.h"

using namespace RayTracer;

TEST(TestPattern, TestStripeCreate) {
  Pattern p(white, black);
  EXPECT_EQ(white, p.colors().first);
  EXPECT_EQ(black, p.colors().second);
}

TEST(TestPattern, TestStripeConstY) {
  Pattern p(white, black);
  EXPECT_EQ(white, p.color_at(Point(0, 0, 0)));
  EXPECT_EQ(white, p.color_at(Point(0, 1, 0)));
  EXPECT_EQ(white, p.color_at(Point(0, 2, 0)));
}

TEST(TestPattern, TestStripeConstZ) {
  Pattern p(white, black);
  EXPECT_EQ(white, p.color_at(Point(0, 0, 0)));
  EXPECT_EQ(white, p.color_at(Point(0, 0, 1)));
  EXPECT_EQ(white, p.color_at(Point(0, 0, 2)));
}

TEST(TestPattern, TestStripeVarX) {
  Pattern p(white, black);
  EXPECT_EQ(white, p.color_at(Point(0, 0, 0)));
  EXPECT_EQ(white, p.color_at(Point(0.9f, 0, 0)));
  EXPECT_EQ(black, p.color_at(Point(1, 0, 0)));
  EXPECT_EQ(black, p.color_at(Point(-0.1f, 0, 0)));
  EXPECT_EQ(black, p.color_at(Point(-1, 0, 0)));
  EXPECT_EQ(white, p.color_at(Point(-1.1f, 0, 0)));
}

TEST(TestPattern, TestStripeAtObjectTransform) {
  Sphere object(Transform::id().scale(2, 2, 2));
  Pattern p(white, black);
  ASSERT_EQ(white, p.color_at_object(std::ref(object), Point(1.5f, 0, 0)));
}

TEST(TestPattern, TestStripeAtStripeTransform) {
  Sphere object;
  Pattern p(white, black, Transform::id().scale(2, 2, 2));
  ASSERT_EQ(white, p.color_at_object(std::ref(object), Point(1.5f, 0, 0)));
}

TEST(TestPattern, TestStripeAtObjectAndPatternTransform) {
  Sphere object(Transform::id().scale(2, 2, 2));
  Pattern p(white, black, Transform::id().translate(0.5f, 0, 0));
  ASSERT_EQ(white, p.color_at_object(std::ref(object), Point(2.5f, 0, 0)));
}
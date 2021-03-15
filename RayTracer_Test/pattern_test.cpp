#include "pch.h"

#include "../RayTracerLib/Pattern.h"
#include "../RayTracerLib/Tuple.h"
#include "../RayTracerLib/Sphere.h"

using namespace RayTracer;

class TestPattern : public Pattern {
public:
  TestPattern(Transform t = Transform::id()) : Pattern(t) {}
  virtual Color color_at(Point p) const override {
    return Color(p.x, p.y, p.z);
  }
};

TEST(TestPattern, TestCustomTransform) {
  TestPattern p(Transform::id().translate(1, 2, 3));
  ASSERT_EQ(Transform::id().translate(1, 2, 3), p.transform());
}

TEST(TestPattern, TestPatternAtObjectTransform) {
  Sphere object(Transform::id().scale(2, 2, 2));
  TestPattern p;
  ASSERT_EQ(Color(1, 1.5f, 2), p.color_at_object(std::ref(object), Point(2, 3, 4)));
}

TEST(TestPattern, TestPatternAtPatternTransform) {
  Sphere object;
  TestPattern p(Transform::id().scale(2, 2, 2));
  ASSERT_EQ(Color(1, 1.5f, 2), p.color_at_object(std::ref(object), Point(2, 3, 4)));
}

TEST(TestPattern, TestPatternAtObjectAndPatternTransform) {
  Sphere object(Transform::id().scale(2, 2, 2));
  TestPattern p(Transform::id().translate(0.5f, 1, 1.5f));
  ASSERT_EQ(Color(0.75f, 0.5f, 0.25f), p.color_at_object(std::ref(object), Point(2.5f, 3, 3.5f)));
}

TEST(TestPattern, TestStripeCreate) {
  StripePattern p(white, black);
  EXPECT_EQ(white, p.colors().first);
  EXPECT_EQ(black, p.colors().second);
}

TEST(TestPattern, TestStripeConstY) {
  StripePattern p(white, black);
  EXPECT_EQ(white, p.color_at(Point(0, 0, 0)));
  EXPECT_EQ(white, p.color_at(Point(0, 1, 0)));
  EXPECT_EQ(white, p.color_at(Point(0, 2, 0)));
}

TEST(TestPattern, TestStripeConstZ) {
  StripePattern p(white, black);
  EXPECT_EQ(white, p.color_at(Point(0, 0, 0)));
  EXPECT_EQ(white, p.color_at(Point(0, 0, 1)));
  EXPECT_EQ(white, p.color_at(Point(0, 0, 2)));
}

TEST(TestPattern, TestStripeVarX) {
  StripePattern p(white, black);
  EXPECT_EQ(white, p.color_at(Point(0, 0, 0)));
  EXPECT_EQ(white, p.color_at(Point(0.9f, 0, 0)));
  EXPECT_EQ(black, p.color_at(Point(1, 0, 0)));
  EXPECT_EQ(black, p.color_at(Point(-0.1f, 0, 0)));
  EXPECT_EQ(black, p.color_at(Point(-1, 0, 0)));
  EXPECT_EQ(white, p.color_at(Point(-1.1f, 0, 0)));
}

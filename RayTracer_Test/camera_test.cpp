#include "pch.h"
#include "../RayTracerLib/Camera.h"

using namespace RayTracer;

TEST(TestCamera, TestCameraCreate) {
  Camera c(std::make_pair(160, 120), M_PI_2);

  EXPECT_EQ(160, c.hsize());
  EXPECT_EQ(120, c.vsize());
  EXPECT_TRUE(ApproxEqual(M_PI_2, c.fov(), epsilon));
  EXPECT_EQ(Transform::id(), c.view());
}

TEST(TestCamera, TestPixelSizeHorizCanvas) {
  Camera c(std::make_pair(200, 125), M_PI_2);
  ASSERT_TRUE(ApproxEqual(0.01, c.pixel_size(), epsilon));
}

TEST(TestCamera, TestPixelSizeVertCanvas) {
  Camera c(std::make_pair(125, 200), M_PI_2);
  ASSERT_TRUE(ApproxEqual(0.01, c.pixel_size(), epsilon));
}

TEST(TestCamera, TestRayThroughCenter) {
  Camera c(std::make_pair(201, 101), M_PI_2);
  auto r = c.ray_for_pixel(100, 50);
  EXPECT_EQ(Point(0, 0, 0), r.origin());
  EXPECT_EQ(Vector(0, 0, -1), r.direction());
}

TEST(TestCamera, TestRayThroughCorner) {
  Camera c(std::make_pair(201, 101), M_PI_2);
  auto r = c.ray_for_pixel(0, 0);
  EXPECT_EQ(Point(0, 0, 0), r.origin());
  EXPECT_EQ(Vector(0.66519f, 0.33259f, -0.66851f), r.direction());
}

TEST(TestCamera, TestRayCameraTransformed) {
  Camera c(std::make_pair(201, 101), M_PI_2, Transform::id().translate(0, -2, 5).rot_y(M_PI_4));
  auto r = c.ray_for_pixel(100, 50);
  EXPECT_EQ(Point(0, 2, -5), r.origin());
  EXPECT_EQ(Vector(sqrtf(2) / 2.0f, 0, -sqrtf(2) / 2.0f), r.direction());
}

TEST(TestCamera, TestRender) {
  auto w = World::default_world();
  Point from(0, 0, -5), to(0, 0, 0);
  Vector up(0, 1, 0);
  Camera c(std::make_pair(11, 11), M_PI_2, Transform::id().view(from, to, up));
  auto image = c.render(*w);
  EXPECT_EQ(Color(0.38066f, 0.47583f, 0.2855f), image.pixel(5, 5));
}
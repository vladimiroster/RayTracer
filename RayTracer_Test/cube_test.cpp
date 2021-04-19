#include "pch.h"

#include "../RayTracerLib/Cube.h"

using namespace RayTracer;

TEST(TestCube, TestRayIntersects) {
  Cube c;

  // TODO: parametrized test
  std::array<Point, 7> origin{
    Point(5, 0.5f, 0), // +x
    Point(-5, 0.5f, 0), // -x
    Point(0.5f, 5, 0), // +y
    Point(0.5f, -5, 0), // -y
    Point(0.5f, 0, 5), // +z
    Point(0.5f, 0, -5), // -z
    Point(0, 0.5f, 0), // inside
  };

  std::array<Vector, 7> direction{
    Vector(-1, 0, 0), // +x
    Vector(1, 0, 0), // -x
    Vector(0, -1, 0), // +y
    Vector(0, 1, 0), // -y
    Vector(0, 0, -1), // +z
    Vector(0, 0, 1), // -z
    Vector(0, 0, 1), // inside
  };

  std::array<float, 7> t1{
    4, 4, 4, 4, 4, 4, -1
  };

  std::array<float, 7> t2{
    6, 6, 6, 6, 6, 6, 1
  };

//  for (int i = 0; i < origin.size(); ++i) {
  int i = 0;
    Ray r(origin[i], direction[i]);
    auto xs = c.intersect_local(r);
    ASSERT_EQ(2, xs.size());
    EXPECT_EQ(t1[i], xs[0].time());
    EXPECT_EQ(t2[i], xs[1].time());
  //}
}

TEST(TestCube, TestRayMiss) {
  Cube c;

  // TODO: parametrized test
  std::array<Point, 6> origin{
    Point(-2, 0, 0),
    Point(0, -2, 0),
    Point(0, 0, -2),
    Point(2, 0, 2),
    Point(0, 2, 2),
    Point(2, 2, 0),
  };

  std::array<Vector, 6> direction{
    Vector(0.2673f, 0.5345f, 0.8018f),
    Vector(0.8018f, 0.2673f, 0.5345f),
    Vector(0.5345f, 0.8018f, 0.2673f),
    Vector(0, 0, -1),
    Vector(0, -1, 0),
    Vector(-1, 0, 0),
  };

  for (int i = 0; i < origin.size(); ++i) {
    Ray r(origin[i], direction[i]);
    auto xs = c.intersect_local(r);
    EXPECT_EQ(0, xs.size());
  }
}

TEST(TestCube, TestNormal) {
  Cube c;

  std::array<Point, 8> points{
    Point(1, 0.5f, -0.8f),
    Point(-1, -0.2f, 0.9f),
    Point(-0.4f, 1, -0.1f),
    Point(0.3f, -1, -0.7f),
    Point(-0.6f, 0.3f, 1),
    Point(0.4f, 0.4f, -1),
    Point(1, 1, 1),
    Point(-1, -1, -1)
  };

  std::array<Vector, 8> normals{
    Vector(1, 0, 0),
    Vector(-1, 0, 0),
    Vector(0, 1, 0),
    Vector(0, -1, 0),
    Vector(0, 0, 1),
    Vector(0, 0, -1),
    Vector(1, 0, 0),
    Vector(-1, 0, 0),
  };

  for (int i = 0; i < points.size(); ++i) {
    auto normal = c.normal_local(points[i]);
    EXPECT_EQ(normals[i], normal);
  }
}
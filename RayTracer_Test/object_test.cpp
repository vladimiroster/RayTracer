#include "pch.h"

#include "../RayTracerLib/Object.h"
#include "../RayTracerLib/Transformation.h"

using namespace RayTracer;

class TestShape : public Object {
public:
  TestShape(Transform t = Transform::id(), Material m = Material()) : Object(t, m)
  {}

  std::unique_ptr<Ray> saved_ray = nullptr;

protected:
  virtual std::vector<Intersection> intersect_local(const Ray& r) const override{
    // Some ungodly things for testing
    TestShape* this2 = const_cast<TestShape*>(this);
    this2->saved_ray = std::make_unique<Ray>(r);

    return {};
  }

  virtual Vector normal_local(const Point& p) const override{
    return Vector(p.x, p.y, p.z);
  }

};

TEST(TestObject, TestObjectCreate) {
  TestShape shape;
  EXPECT_EQ(Transform::id(), shape.transform());
}

TEST(TestObject, TestChangeObjectTransform) {
  TestShape s(Transform::id().translate(2, 3, 4));
  EXPECT_EQ(Transform::id().translate(2, 3, 4), s.transform());
  EXPECT_EQ(Transform::id().translate(2, 3, 4).inverse(), s.inverse());
}

TEST(TestObject, TestDefaultMaterial) {
  TestShape s;
  EXPECT_EQ(Material(), s.material());
}

TEST(TestObject, TestCustomMaterial) {
  TestShape s;
  Material m;
  m.ambient = 1;
  s.set_material(m);
  EXPECT_EQ(m, s.material());
}

TEST(TestObject, TestScaledShapeIntersect) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  TestShape s(Transform::id().scale(2, 2, 2));

  s.intersect(r);
  EXPECT_EQ(Point(0, 0, -2.5f), s.saved_ray->origin());
  EXPECT_EQ(Vector(0, 0, 0.5f), s.saved_ray->direction());
}

TEST(TestObject, TestTranslatedShapeIntersect) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  TestShape s(Transform::id().translate(5, 0, 0));

  s.intersect(r);
  EXPECT_EQ(Point(-5, 0, -5), s.saved_ray->origin());
  EXPECT_EQ(Vector(0, 0, 1), s.saved_ray->direction());
}

TEST(TestObject, TestTranslatedNormal) {
  TestShape s(Transform::id().translate(0, 1, 0));
  auto n = s.normal(Point(0, 1.70711f, -0.70711f));
  EXPECT_EQ(Vector(0, 0.70711f, -0.70711f), n);
}

TEST(TestObject, TestTransformedNormal) {
  TestShape s(Transform::id().rot_z(M_PI / 5.0f).scale(1, 0.5f, 1));
  auto n = s.normal(Point(0, sqrtf(2)/2, -sqrtf(2)/2));
  EXPECT_EQ(Vector(0, 0.97014f, -0.24254f), n);
}

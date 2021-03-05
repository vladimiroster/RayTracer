#include "pch.h"

#include "../RayTracerLib/Tuple.h"
#include "../RayTracerLib/Lighting.h"

using namespace RayTracer;

TEST(TestLighting, TestPointLightCreate) {
  Light light(Color(1, 1, 1), Point(0, 0, 0));
  EXPECT_EQ(Color(1, 1, 1), light.intensity);
  EXPECT_EQ(Point(0, 0, 0), light.position);
}

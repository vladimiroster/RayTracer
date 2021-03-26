//
// pch.h
// Header for standard system include files.
//

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "gtest/gtest.h"

#include "../RayTracerLib/Pattern.h"

class TestPattern : public RayTracer::Pattern {
public:
  TestPattern(RayTracer::Transform t = RayTracer::Transform::id()) : RayTracer::Pattern(t) {}
  virtual RayTracer::Color color_at(RayTracer::Point p) const override {
    return RayTracer::Color(p.x, p.y, p.z);
  }
};

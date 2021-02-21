// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "../RayTracerLib/Tuple.h"
#include "../RayTracerLib/Canvas.h"
#include "../RayTracerLib/Matrix.h"
#include "../RayTracerLib/Transformation.h"

#include "Elements.h"

namespace rt = RayTracer;

void projectile();
void clock();

int main()
{
  clock();
}

void projectile() {
  Projectile p(rt::Point(0.0f, 1.0f, 0.0f), rt::normalize(rt::Vector(1.0f, 1.8f, 0.0f)) * 11.25);
  Environment env(rt::Vector(0.0f, -0.1f, 0.0f), rt::Vector(-0.01f, 0.0f, 0.0f));
  rt::Canvas c(900, 550);

  int time = 0;
  while (p.position.y > 0) {
    p = tick(env, p);
    if (rt::isValidCoordinate(c, p.position)) {
      c.pixel(static_cast<size_t>(p.position.x), static_cast<size_t>(c.height - p.position.y)) = rt::Color(1.0f, 1.0f, 1.0f);
    }
    else {
      std::cout << "Not plotting: ";
    }
    std::cout << time << ": " << p.position << std::endl;
    ++time;
  }

  std::ofstream of("c:\\temp\\projectile.ppm");
  of << c;
}

void clock() {
  // Parameters to play with
  constexpr size_t CANVAS_SIZE = 300;
  constexpr float BORDER = 1.0f/5.0f;
  constexpr size_t NUM_DIALS = 12;
  constexpr float ARC = 3.1415f * 2 / NUM_DIALS;

  rt::Canvas c(CANVAS_SIZE, CANVAS_SIZE );

  // World to canvas transform
  auto w2c = rt::Transform::id().translate(CANVAS_SIZE / 2, CANVAS_SIZE / 2, 0);

  std::vector<rt::Point> dials;

  // Place the first dial
  dials.emplace_back(0.0f, -(CANVAS_SIZE / 2.0f * (1 - BORDER)), 0.0f);

  auto T = rt::Transform::id().rot_z(ARC);
  for (size_t i = 1; i < NUM_DIALS; ++i) {
    // Take the last dial and rotate by arc
    dials.emplace_back(T * dials.back());
  }

  for (auto p : dials) {
    // Convert to canvas space
    auto cp = w2c * p;
    c.pixel(static_cast<size_t>(cp.x), static_cast<size_t>(cp.y)) = rt::Color(1.0f, 1.0f, 1.0f);
  }

  std::ofstream of("c:\\temp\\clock.ppm");
  of << c;
}

// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include "Tuple.h"
#include "Canvas.h"

#include "Elements.h"

namespace rt = RayTracer;

int main()
{
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

  std::ofstream of("c:\\temp\\canvas.ppm");
  of << c;
}

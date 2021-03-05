// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

#include "../RayTracerLib/Tuple.h"
#include "../RayTracerLib/Canvas.h"
#include "../RayTracerLib/Matrix.h"
#include "../RayTracerLib/Transformation.h"
#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Materials.h"

#include "Elements.h"

namespace rt = RayTracer;

void projectile();
void clockface();
void sphere(float sphere_radius_x, float sphere_radius_y, float z_ang, rt::Material material, rt::Light light, size_t id);
void spheres();

int main()
{
  // TODO: organize examples
  spheres();
}

void projectile() {
  Projectile p(rt::Point(0.0f, 1.0f, 0.0f), rt::normalize(rt::Vector(1.0f, 1.8f, 0.0f)) * 11.25);
  Environment env(rt::Vector(0.0f, -0.1f, 0.0f), rt::Vector(-0.01f, 0.0f, 0.0f));
  rt::Canvas c(900, 550);

  int time = 0;
  while (p.position.y > 0) {
    p = tick(env, p);
    if (c.isValidCoordinate(p.position)) {
      c.pixel(static_cast<size_t>(p.position.x), static_cast<size_t>(c.height() - p.position.y)) = rt::Color(1.0f, 1.0f, 1.0f);
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

void clockface() {
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

void sphere(float sphere_radius_x, float sphere_radius_y, float z_ang, rt::Material material, rt::Light light, size_t id) {
  // Parameters to play with
  constexpr size_t CANVAS_WIDTH = 100;
  constexpr size_t CANVAS_HEIGHT = 100;

  // World to canvas transform
  // In conjunction with the camera location, that defines the viewport
  auto w2c = rt::Transform::id().translate(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 0);
  auto c2w = w2c.inverse();
  const rt::Point CAMERA_LOCATION(0, 0, -100);

  // Sphere's transform
  const rt::Transform T = rt::Transform::id()
    .scale(sphere_radius_x, sphere_radius_y, 1)
    .rot_z(z_ang);

  rt::Canvas c(CANVAS_WIDTH, CANVAS_HEIGHT);
  rt::Sphere s(T);
  s.set_material(material);

  for (size_t i = 0; i < CANVAS_WIDTH; ++i) {
    for (size_t j = 0; j < CANVAS_HEIGHT; ++j) {
      // Convert each canvas point to world space, and run a ray to that point
      rt::Ray r(CAMERA_LOCATION, normalize(c2w * rt::Point(static_cast<float>(i), static_cast<float>(j), 0) - CAMERA_LOCATION));
      auto h = rt::hit(s.intersect(r));
      if (h) {
        auto point = r.position(h->time());
        auto normal = h->object().normal(point);
        rt::Vector eye = -r.direction();
        c.pixel(i, j) = rt::lighting(h->object().material(), light, point, eye, normal);
      }
    }
  }

  std::stringstream strm;
  strm << "c:\\temp\\sphere\\" << std::setfill('0') << std::setw(5) << id << ".ppm";
  std::ofstream of(strm.str());
  of << c;
}

void spheres() {
  // TODO: play around with light and material values
  size_t w = 50, h = 50, idx = 0;
  float z_ang = 0;
  rt::Material m;
  m.color = rt::Color(1, 0.f, 1);
  rt::Light light(rt::white, rt::Point(-10, 10, -10));

  for (int i = 0; i < 10; ++i) {
    w -= 2;
    h -= 2;
    light.position.x += 2;
    sphere(static_cast<float>(w), static_cast<float>(h), 0, m, light, ++idx);
  }

  for (int i = 0; i < 10; ++i) {
    w += 2;
    light.position.y += 2;
    sphere(static_cast<float>(w), static_cast<float>(h), 0, m, light, ++idx);
  }

  for (int i = 0; i < 10; ++i) {
    z_ang += 3.14159f / 20;
    light.position.x -= 2;
    sphere(static_cast<float>(w), static_cast<float>(h), z_ang, m, light, ++idx);
  }

  for (int i = 0; i < 10; ++i) {
    h += 2;
    light.position.y -= 2;
    sphere(static_cast<float>(w), static_cast<float>(h), z_ang, m, light, ++idx);
  }

  // Make a GIF:
  //
  // magick mogrify -format jpg *.ppm
  // magick convert -delay 10 -loop 0 *.jpg sphere.gif
}

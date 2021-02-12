#pragma once

#include <iostream>
#include <algorithm>

#include "Tuple.h"

namespace RayTracer {
  struct Canvas {
    size_t width = 0;
    size_t height = 0;
    Color *p = nullptr;

    Canvas(size_t w, size_t h) :
      width(w), height(h) {
      p = new Color[width * height];
    }

    ~Canvas() {
      if (p) {
        delete[] p;
      }
    }

    size_t size() const {
      return width * height;
    }

    Color& pixel(size_t i, size_t j) {
      return p[i + width * j];
    }
  };

  bool isValidCoordinate(const Canvas& c, Point p) {
    return p.x >= 0 &&
      p.x <= c.width &&
      p.y >= 0 &&
      p.y <= c.height;
  }

  int toOutputColor(float c) {
    // First clamp it between 0 and one
    c = std::max(0.0f, std::min(c, 1.0f));
    return static_cast<int>(std::round(c * 255.0f));
  }

  std::ostream& operator<<(std::ostream& os, const Canvas& c) {
    os << "P3" << std::endl;
    os << c.width << " " << c.height << std::endl;
    os << "255" << std::endl;
    for (size_t i = 0; i < c.width * c.height; ++i) {
      os << toOutputColor(c.p[i].x) << " " << toOutputColor(c.p[i].y) << " " << toOutputColor(c.p[i].z);
      if (i % c.width == c.width - 1) {
        os << std::endl;
      }
      else {
        os << " ";
      }
    }
    return os;
  }
}
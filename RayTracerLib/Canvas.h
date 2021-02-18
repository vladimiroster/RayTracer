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

  bool isValidCoordinate(const Canvas& c, Point p);
  int toOutputColor(float c);
  std::ostream& operator<<(std::ostream& os, const Canvas& c);

} // namespace RayTracer
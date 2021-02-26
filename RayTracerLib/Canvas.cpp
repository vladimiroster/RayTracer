#include "Canvas.h"

namespace RayTracer {
  bool Canvas::isValidCoordinate(Point p) const {
    return p.x >= 0 &&
      p.x <= _width &&
      p.y >= 0 &&
      p.y <= _height;
  }

  int toOutputColor(float c) {
    // First clamp it between 0 and one
    c = std::max(0.0f, std::min(c, 1.0f));
    return static_cast<int>(std::round(c * 255.0f));
  }

  std::ostream& operator<<(std::ostream& os, const Canvas& c) {
    os << "P3" << "\n";
    os << c._width << " " << c._height << "\n";
    os << "255" << "\n";
    for (size_t i = 0; i < c._width * c._height; ++i) {
      os << toOutputColor(c._canvas[i].x) << " " << toOutputColor(c._canvas[i].y) << " " << toOutputColor(c._canvas[i].z);
      if (i % c._width == c._width - 1) {
        os << "\n";
      }
      else {
        os << " ";
      }
    }
    return os;
  }
} // namespace RayTracer

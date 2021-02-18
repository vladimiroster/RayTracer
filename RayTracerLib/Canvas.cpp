#include "Canvas.h"

namespace RayTracer {
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
} // namespace RayTracer

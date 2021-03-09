#include "Camera.h"
#include "Profiler.h"

namespace RayTracer {
  Camera::Camera(size_t hsize, size_t vsize, float fov, Transform view) : _hsize(hsize), _vsize(vsize), _fov(fov), _view(view), _inverse(view.inverse())
  {
    auto half_view = tanf(fov / 2.0f);
    float aspect = static_cast<float>(_hsize) / _vsize;
   
    if (aspect >= 1) {
      _half_width = half_view;
      _half_height = half_view / aspect;
    }
    else {
      _half_width = half_view * aspect;
      _half_height = half_view;
    }
    _pixel_size = _half_width * 2 / _hsize;
  }

  Ray Camera::ray_for_pixel(size_t x, size_t y) const
  {
    // Offset from the edge of the canvas to the pixel's center
    auto xoffset = (x + 0.5) * _pixel_size;
    auto yoffset = (y + 0.5) * _pixel_size;

    // Pixel coordinates in world space
    auto world_x = _half_width - xoffset;
    auto world_y = _half_height - yoffset;

    // Transform the canvas point and the origin (canvas is at z=-1)
    auto pixel = _inverse * Point(world_x, world_y, -1);
    Point origin = _inverse * Point(0, 0, 0);
    Vector direction = normalize(pixel - origin);

    return Ray(origin, direction);
  }

  Canvas Camera::render(const World & w) const
  {
    auto profile = profiler.profile(__FUNCDNAME__);

    Canvas c(_hsize, _vsize);

    // MS version of OpenMP still requires the loop variable to be signed
    #pragma omp parallel for
    for (long long x = 0; x < _hsize; ++x) {
      for (size_t y = 0; y < _vsize; ++y) {
        auto r = ray_for_pixel(x, y);
        auto color = w.color_at(r);
        c.pixel(x, y) = color;
      }
    }

    return std::move(c);
  }

} // namespace RayTracer
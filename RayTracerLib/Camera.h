#pragma once

#include "Transformation.h"
#include "Ray.h"
#include "World.h"
#include "Canvas.h"

namespace RayTracer {
  class Camera {
  public:
    Camera(std::pair<size_t, size_t> resolution, float fov, Transform view = Transform::id());

    inline static constexpr std::pair<size_t, size_t> RES_TINY = std::make_pair(100, 50);
    inline static constexpr std::pair<size_t, size_t> RES_640X480 = std::make_pair(640, 480);
    inline static constexpr std::pair<size_t, size_t> RES_720P = std::make_pair(1280, 720);
    inline static constexpr std::pair<size_t, size_t> RES_1080P = std::make_pair(1920, 1080);
    inline static constexpr std::pair<size_t, size_t> RES_UHD = std::make_pair(3840, 2160);
    inline static constexpr std::pair<size_t, size_t> RES_4K = std::make_pair(4096, 2160);
    inline static constexpr std::pair<size_t, size_t> RES_FULL4K = std::make_pair(4096, 4096);

    size_t hsize() const {
      return _hsize;
    }

    size_t vsize() const {
      return _vsize;
    }

    float fov() const {
      return _fov;
    }

    const Transform& view() const {
      return _view;
    }

    float pixel_size() const {
      return _pixel_size;
    }

    Ray ray_for_pixel(size_t x, size_t y) const;

    Canvas render(const World& w) const;

  private:
    size_t _hsize;
    size_t _vsize;
    float _fov;
    Transform _view;
    Transform _inverse;
    float _pixel_size;
    float _half_width;
    float _half_height;
  };
} // namespace RayTracer
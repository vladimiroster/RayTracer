#pragma once

#include "Transformation.h"
#include "Ray.h"
#include "World.h"
#include "Canvas.h"

namespace RayTracer {
  class Camera {
  public:
    Camera(size_t hsize, size_t vsize, float fov, Transform view = Transform::id());

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
}
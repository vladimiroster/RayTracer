#pragma once

#include <iostream>
#include <algorithm>

#include "Tuple.h"

namespace RayTracer {
  class Canvas {
    friend std::ostream& operator<<(std::ostream& os, const Canvas& c);

  private:
    size_t _width = 0;
    size_t _height = 0;
    Color *_canvas = nullptr;

  public:
    Canvas(size_t w, size_t h) :
      _width(w), _height(h) {
      _canvas = new Color[_width * _height];
    }

    Canvas(Canvas&& other) {
      _width = other._width;
      _height = other._height;
      _canvas = other._canvas;
      other._width = other._height = 0;
      other._canvas = nullptr;
    }

    ~Canvas() {
      if (_canvas) {
        delete[] _canvas;
        _canvas = nullptr;
      }
    }

    size_t width() const {
      return _width;
    }

    size_t height() const {
      return _height;
    }

    size_t size() const {
      return _width * _height;
    }

    Color& pixel(size_t i, size_t j) {
      return _canvas[i + _width * j];
    }

    const Color& pixel(size_t i, size_t j) const {
      return _canvas[i + _width * j];
    }

    const Color* canvas() const {
      return _canvas;
    }

    bool isValidCoordinate(Point p) const;
  };

  
  int toOutputColor(float c);
  std::ostream& operator<<(std::ostream& os, const Canvas& c);

} // namespace RayTracer
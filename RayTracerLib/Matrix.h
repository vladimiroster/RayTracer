#pragma once
#include <array>

#include "Tuple.h"

namespace RayTracer {

  template<size_t n>
  class Matrix {
  private:
    std::array<std::array<float, n>, n> _data;

  public:
    Matrix() = default;
    Matrix(std::array<std::array<float, n>, n> init) : _data(init) {}

    std::array<float, n>& operator[](size_t row) {
      return _data[row];
    }

    const std::array<float, n>& operator[](size_t row) const {
      return _data[row];
    }

    std::array<float, n> col(size_t c) const {
      std::array<float, n> arr;
      for (size_t i = 0; i < n; ++i) {
        arr[i] = _data[i][c];
      }
      return arr;
    }

    bool operator==(const Matrix<n>& rhs) const;
    bool operator!=(const Matrix<n>& rhs) const;
    Matrix<n> operator*(const Matrix<n>& other) const;
    Matrix<n> transpose() const;
    Matrix<n-1> submatrix(size_t row, size_t col) const;
    float minor(size_t row, size_t col) const;
    float cofactor(size_t row, size_t col) const;
    float det() const;
    // TODO: https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    Matrix<n> inverse() const;
  };

  template<size_t n>
  inline bool Matrix<n>::operator==(const Matrix<n>& other) const
  {
    bool equal = true;

    // TODO: verify optimizer unrolls the loop
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        if (!ApproxEqual(_data[i][j], other[i][j], epsilon)) {
          return false;
        }
      }
    }

    return true;
  }

  template<size_t n>
  inline bool Matrix<n>::operator!=(const Matrix<n>& other) const
  {
    return !(*this == other);
  }

  template<size_t n>
  inline Matrix<n> Matrix<n>::operator*(const Matrix<n>& other) const
  {
    Matrix<n> result;

    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        result[i][j] = dot((*this)[i], other.col(j));
      }
    }
    return result;
  }

  template<size_t n>
  inline Matrix<n> Matrix<n>::transpose() const
  {
    Matrix<n> result;

    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        result[i][j] = _data[j][i];
      }
    }
    return result;
  }

  template<size_t n>
  inline Matrix<n - 1> Matrix<n>::submatrix(size_t row, size_t col) const
  {
    Matrix<n-1> result;

    for (size_t i = 0, new_i = 0; i < n; ++i) {
      if (i == row) {
        continue;
      }
      for (size_t j = 0, new_j = 0; j < n; ++j) {
        if (j == col) {
          continue;
        }
        result[new_i][new_j] = _data[i][j];
        ++new_j;
      }
      ++new_i;
    }

    return result;
  }

  template<size_t n>
  inline float Matrix<n>::minor(size_t row, size_t col) const
  {
    return submatrix(row, col).det();
  }

  template<size_t n>
  inline float Matrix<n>::cofactor(size_t row, size_t col) const
  {
    return minor(row, col) * ((row + col) % 2 ? -1 : 1);
  }

  template<size_t n>
  inline float Matrix<n>::det() const
  {
    float det = 0;
    for (size_t i = 0; i < n; ++i) {
      det += _data[0][i] * cofactor(0, i);
    }
    return det;
  }

  template<>
  inline float Matrix<2>::det() const {
    return _data[0][0] * _data[1][1] - _data[0][1] * _data[1][0];
  }


  template<size_t n>
  inline Matrix<n> Matrix<n>::inverse() const
  {
    float d = det();
    if (d == 0) {
      throw std::invalid_argument("Trying to invert a singular matrix");
    }

    Matrix<n> result;

    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        float c = cofactor(i, j);
        result[j][i] = c/d;
      }
    }

    return result;
  }

  // TODO: make templated and constexpr
  inline const Matrix<4> id4({1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1});

  // Operations on specific matrix sizes
  Tuple operator*(const Matrix<4>& lhs, const Tuple& rhs);
} // namespace RayTracer
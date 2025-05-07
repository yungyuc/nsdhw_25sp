#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>

template <typename T>
class CustomAllocator {
public:
  using value_type = T;

  CustomAllocator() = default;

  template <typename U>
  CustomAllocator(const CustomAllocator<U> &) {};

  T *allocate(std::size_t n) {
    if (n > std::size_t(-1) / sizeof(T)) {
      throw std::bad_alloc();
    }
    const std::size_t bytes = n * sizeof(T);
    if (auto p = static_cast<T *>(std::malloc(n * sizeof(T)))) {
      allocated_bytes += bytes;
      return p;
    }
    throw std::bad_alloc();
  }

  void deallocate(T *p, std::size_t n) noexcept {
    std::free(p);
    const std::size_t bytes = n * sizeof(T);
    deallocated_bytes += bytes;
  }

  static std::size_t get_allocated_bytes() {
    return allocated_bytes;
  }

  static std::size_t get_deallocated_bytes() {
    return deallocated_bytes;
  }

  static std::size_t get_bytes() {
    return allocated_bytes - deallocated_bytes;
  }

  static std::size_t allocated_bytes;
  static std::size_t deallocated_bytes;
};

template <typename T>
std::size_t CustomAllocator<T>::allocated_bytes = 0;
template <typename T>
std::size_t CustomAllocator<T>::deallocated_bytes = 0;

std::size_t bytes();
std::size_t allocated();
std::size_t deallocated();

class Matrix {
public:
  std::vector<double, CustomAllocator<double>> mat;
  Matrix(int r = 1, int c = 1) {
      row = r;
      col = c;
      mat = std::vector<double, CustomAllocator<double>>(row * col, 0.);
  }
  double &operator()(std::size_t i, std::size_t j) {
    // setter
    return mat[i * col + j];
  }
  const double &operator()(std::size_t i, std::size_t j) const {
    // getter
    return mat[i * col + j];
  }
  std::size_t nrow() const {
    return row;
  }
  std::size_t ncol() const {
    return col;
  }
private:
  std::size_t row;
  std::size_t col;
};

Matrix multiply_naive(const Matrix &m1, const Matrix &m2);
Matrix multiply_tile(Matrix &m1, Matrix &m2, int tsize);
Matrix multiply_mkl(const Matrix &m1, const Matrix &m2);

bool operator==(const Matrix &m1, const Matrix &m2);

#endif // MATRIX_HPP
#include <iostream>
#include <iomanip>
#include <vector>
#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#else
#ifdef USE_MKL
#include <mkl.h>
#else
#include <cblas.h>
#endif
#endif

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Matrix
{
public:
  Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
  {
    size_t nelement = nrow * ncol;
    m_buffer = new double[nelement](); // 初始化為0
  }

  ~Matrix()
  {
    delete[] m_buffer;
  }

  Matrix(const Matrix &other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
  {
    size_t nelement = m_nrow * m_ncol;
    m_buffer = new double[nelement];
    std::copy(other.m_buffer, other.m_buffer + nelement, m_buffer);
  }

  Matrix &operator=(const Matrix &other)
  {
    if (this != &other)
    {
      if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
      {
        delete[] m_buffer;
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
        m_buffer = new double[m_nrow * m_ncol];
      }
      std::copy(other.m_buffer, other.m_buffer + m_nrow * m_ncol, m_buffer);
    }
    return *this;
  }

  // No bound check.
  double operator()(size_t row, size_t col) const
  {
    return m_buffer[row * m_ncol + col];
  }

  double &operator()(size_t row, size_t col)
  {
    return m_buffer[row * m_ncol + col];
  }

  // Get data pointer for BLAS operations
  double *data() { return m_buffer; }
  const double *data() const { return m_buffer; }

  // Python-style indexing for pybind11
  double get(size_t row, size_t col) const
  {
    return m_buffer[row * m_ncol + col];
  }

  void set(size_t row, size_t col, double val)
  {
    m_buffer[row * m_ncol + col] = val;
  }

  bool operator==(const Matrix &other) const
  {
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
    {
      return false;
    }

    for (size_t i = 0; i < m_nrow; ++i)
    {
      for (size_t j = 0; j < m_ncol; ++j)
      {
        if ((*this)(i, j) != other(i, j))
        {
          return false;
        }
      }
    }
    return true;
  }

  size_t nrow() const { return m_nrow; }
  size_t ncol() const { return m_ncol; }

private:
  size_t m_nrow;
  size_t m_ncol;
  double *m_buffer;
};

// Naive matrix multiplication implementation
Matrix multiply_naive(const Matrix &a, const Matrix &b)
{
  if (a.ncol() != b.nrow())
  {
    throw std::runtime_error("Matrix dimensions do not match, cannot multiply");
  }

  Matrix c(a.nrow(), b.ncol());

  for (size_t i = 0; i < a.nrow(); ++i)
  {
    for (size_t j = 0; j < b.ncol(); ++j)
    {
      double sum = 0.0;
      for (size_t k = 0; k < a.ncol(); ++k)
      {
        sum += a(i, k) * b(k, j);
      }
      c(i, j) = sum;
    }
  }

  return c;
}

// Optimized tiled matrix multiplication implementation
Matrix multiply_tile(const Matrix &a, const Matrix &b, int tile_size)
{
  if (a.ncol() != b.nrow())
  {
    throw std::runtime_error("矩陣維度不匹配，無法相乘");
  }

  // If tile_size is 0 or negative, use naive multiplication
  if (tile_size <= 0)
  {
    return multiply_naive(a, b);
  }

  Matrix c(a.nrow(), b.ncol());
  const size_t M = a.nrow();
  const size_t N = b.ncol();
  const size_t K = a.ncol();

  // More optimized tiled implementation: reorder loops to improve cache hit rate
  for (size_t i0 = 0; i0 < M; i0 += tile_size)
  {
    size_t imax = std::min(i0 + tile_size, M);
    for (size_t k0 = 0; k0 < K; k0 += tile_size)
    {
      size_t kmax = std::min(k0 + tile_size, K);

      // Prefetch A submatrix to cache
      for (size_t i = i0; i < imax; ++i)
      {
        for (size_t k = k0; k < kmax; ++k)
        {
          // This loop is only to load part of A into cache
          double prefetch = a(i, k);
          (void)prefetch; // Avoid compiler warning
        }
      }

      for (size_t j0 = 0; j0 < N; j0 += tile_size)
      {
        size_t jmax = std::min(j0 + tile_size, N);

        // Calculate current block
        for (size_t i = i0; i < imax; ++i)
        {
          for (size_t j = j0; j < jmax; ++j)
          {
            // Use temporary storage for each block
            double sum = (k0 == 0) ? 0.0 : c(i, j);
            for (size_t k = k0; k < kmax; ++k)
            {
              sum += a(i, k) * b(k, j);
            }
            c(i, j) = sum;
          }
        }
      }
    }
  }

  return c;
}

// 使用 MKL (BLAS) 的矩陣乘法實現
Matrix multiply_mkl(const Matrix &a, const Matrix &b)
{
  if (a.ncol() != b.nrow())
  {
    throw std::runtime_error("矩陣維度不匹配，無法相乘");
  }

  Matrix c(a.nrow(), b.ncol());

  // DGEMM 參數
  const int m = a.nrow();
  const int n = b.ncol();
  const int k = a.ncol();
  const double alpha = 1.0;
  const double beta = 0.0;
  const int lda = a.ncol();
  const int ldb = b.ncol();
  const int ldc = c.ncol();

  // 使用數據指針調用 BLAS DGEMM 函數
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
              m, n, k, alpha, a.data(), lda, b.data(), ldb, beta, c.data(), ldc);

  return c;
}

PYBIND11_MODULE(_matrix, m)
{
  m.doc() = "矩陣乘法模組";

  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def("__getitem__", [](const Matrix &m, const std::pair<size_t, size_t> &idx)
           { return m.get(idx.first, idx.second); })
      .def("__setitem__", [](Matrix &m, const std::pair<size_t, size_t> &idx, double val)
           { m.set(idx.first, idx.second, val); })
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__eq__", &Matrix::operator==);

  m.def("multiply_naive", &multiply_naive, "朴素矩陣乘法");
  m.def("multiply_tile", &multiply_tile, "分塊矩陣乘法", py::arg("a"), py::arg("b"), py::arg("tile_size"));
  m.def("multiply_mkl", &multiply_mkl, "使用 BLAS/MKL 的矩陣乘法");
}
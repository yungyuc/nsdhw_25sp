#include <vector>
#include <iostream>
#include <cassert>
#include <cblas.h>
#include <pybind11/pybind11.h>
#include "matrix.hpp"

std::size_t bytes() {
  return CustomAllocator<double>::get_bytes();
}

std::size_t allocated() {
  return CustomAllocator<double>::get_allocated_bytes();
}

std::size_t deallocated() {
  return CustomAllocator<double>::get_deallocated_bytes();
}

Matrix multiply_naive(const Matrix &m1, const Matrix &m2) {
  assert(m1.ncol() == m2.nrow());
  int a = m1.nrow();
  int b = m1.ncol();
  int c = m2.ncol();
  Matrix res(a, c);
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < c; ++j) {
      for (int k = 0; k < b; ++k) {
        res.mat[i * c + j] += m1.mat[i * b + k] * m2.mat[k * c + j];
      }
    }
  }
  return res;
}

Matrix multiply_tile(Matrix &m1, Matrix &m2, int tsize) {
  // assert(m1.ncol() == m2.nrow());
  int a = m1.nrow();
  int b = m1.ncol();
  int c = m2.ncol();
  Matrix res(a, c);
  for (int i = 0; i < a; i += tsize) {
    for (int j = 0; j < c; j += tsize) {
      for (int k = 0; k < b; k += tsize) {
        int mina = std::min(a, i + tsize);
        int minb = std::min(b, k + tsize);
        int minc = std::min(c, j + tsize);
        for (int ii = i; ii < mina; ++ii) {
          for (int jj = j; jj < minc; ++jj) {
            double sum = 0;
            for (int kk = k; kk < minb; ++kk) {
              sum += m1.mat[ii * b + kk] * m2.mat[kk * c + jj];
            }
            res.mat[ii * c + jj] += sum;
          }
        }
      }
    }
  }
  return res;
}

Matrix multiply_mkl(const Matrix &m1, const Matrix &m2) {
  assert(m1.ncol() == m2.nrow());
  int a = m1.nrow();
  int b = m1.ncol();
  int c = m2.ncol();
  Matrix res(a, c);

  double *A = new double[a * b];
  double *B = new double[b * c];
  double *C = new double[a * c];
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < b; ++j) {
      A[i * b + j] = m1.mat[i * b + j];
    }
  }
  for (int i = 0; i < b; ++i) {
    for (int j = 0; j < c; ++j) {
      B[i * c + j] = m2.mat[i * c + j];
    }
  }
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, a, c, b, 1.0, A, b, B, c, 0.0, C, c);
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < c; ++j) {
      res.mat[i * c + j] = C[i * c + j];
    }
  }
  delete[] A;
  delete[] B;
  delete[] C;
  return res;
}

bool operator==(const Matrix &m1, const Matrix &m2) {
  if (m1.nrow() != m2.nrow() || m1.ncol() != m2.ncol()) {
    return false;
  }
  for (int i = 0; i < int(m1.nrow() * m1.ncol()); ++i) {
    if (m1.mat[i] != m2.mat[i]) {
      return false;
    }
  }
  return true;
}

// namespace py = pybind11;

// PYBIND11_MODULE(_matrix, m) {
//     m.def("multiply_naive", &multiply_naive);
//     m.def("multiply_tile", &multiply_tile);
//     m.def("multiply_mkl", &multiply_mkl);
//     m.def("bytes", &bytes);
//     m.def("allocated", &allocated);
//     m.def("deallocated", &deallocated);
//     py::class_<Matrix>(m, "Matrix")
//         .def(py::init<int, int>())
//         .def("__setitem__", [](Matrix &m, std::pair<int, int> p, double v) {
//             m.mat[p.first * m.ncol() + p.second] = v;
//         })
//         .def("__getitem__", [](Matrix &m, std::pair<int, int> p) {
//             return m.mat[p.first * m.ncol() + p.second];
//         })
//         .def("__eq__", [](Matrix &m1, Matrix &m2) {
//             return m1.mat == m2.mat;
//         })
//         .def_property_readonly("nrow()", [](const Matrix &m) { return m.nrow(); })
//         .def_property_readonly("ncol()", [](const Matrix &m) { return m.ncol(); })
//         .def("__len__", [](Matrix &m) {
//             return m.nrow();
//         });
// }
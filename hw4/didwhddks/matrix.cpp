#include <vector>
#include <iostream>
#include <cassert>
#include <cblas.h>
#include <pybind11/pybind11.h>

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

std::size_t bytes() {
    return CustomAllocator<double>::get_bytes();
}

std::size_t allocated() {
    return CustomAllocator<double>::get_allocated_bytes();
}

std::size_t deallocated() {
    return CustomAllocator<double>::get_deallocated_bytes();
}

class Matrix {
public:
    int row, col;
    std::vector<double, CustomAllocator<double>> mat;
    Matrix(int r = 1, int c = 1) {
        row = r;
        col = c;
        mat = std::vector<double, CustomAllocator<double>>(r * c, 0.);
    }
};

Matrix multiply_naive(const Matrix &m1, const Matrix &m2) {
    assert(m1.col == m2.row);
    int a = m1.row;
    int b = m1.col;
    int c = m2.col;
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
    assert(m1.col == m2.row);
    int a = m1.row;
    int b = m1.col;
    int c = m2.col;
    Matrix res(a, c);
    for (int i = 0; i < a; i += tsize) {
        for (int j = 0; j < c; j += tsize) {
            for (int k = 0; k < b; k += tsize) {
                for (int ii = i; ii < std::min(a, i + tsize); ++ii) {
                    for (int jj = j; jj < std::min(c, j + tsize); ++jj) {
                        for (int kk = k; kk < std::min(b, k + tsize); ++kk) {
                            res.mat[ii * c + jj] += m1.mat[ii * b + kk] * m2.mat[kk * c + jj];
                        }
                    }
                }
            }
        }
    }
    return res;
}

Matrix multiply_mkl(const Matrix &m1, const Matrix &m2) {
    assert(m1.col == m2.row);
    int a = m1.row;
    int b = m1.col;
    int c = m2.col;
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

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<int, int>())
        .def("__setitem__", [](Matrix &m, std::pair<int, int> p, double v) {
            m.mat[p.first * m.col + p.second] = v;
        })
        .def("__getitem__", [](Matrix &m, std::pair<int, int> p) {
            return m.mat[p.first * m.col + p.second];
        })
        .def("__eq__", [](Matrix &m1, Matrix &m2) {
            return m1.mat == m2.mat;
        })
        .def_property_readonly("nrow", [](const Matrix &m) { return m.row; })
        .def_property_readonly("ncol", [](const Matrix &m) { return m.col; })
        .def("__len__", [](Matrix &m) {
            return m.row;
        });
}
#include <vector>
#include <iostream>
#include <cassert>
#include <cblas.h>
#include <pybind11/pybind11.h>

class Matrix {
public:
    std::vector<std::vector<double>> mat;
    Matrix(int r = 1, int c = 1) {
        mat.assign(r, std::vector<double>(c));
    }
};

Matrix multiply_naive(const Matrix &m1, const Matrix &m2) {
    assert(int(m1.mat[0].size()) == int(m2.mat.size()));
    int a = m1.mat.size();
    int b = m1.mat[0].size();
    int c = m2.mat[0].size();
    Matrix res(a, c);
    for (int i = 0; i < a; ++i) {
        for (int j = 0; j < c; ++j) {
            for (int k = 0; k < b; ++k) {
                res.mat[i][j] += m1.mat[i][k] * m2.mat[k][j];
            }
        }
    }
    return res;
}

Matrix multiply_tile(Matrix &m1, Matrix &m2, int tsize) {
    assert(int(m1.mat[0].size()) == int(m2.mat.size()));
    int a = m1.mat.size();
    int b = m1.mat[0].size();
    int c = m2.mat[0].size();
    Matrix res(a, c);
    for (int i = 0; i < a; i += tsize) {
        for (int j = 0; j < c; j += tsize) {
            for (int k = 0; k < b; k += tsize) {
                for (int ii = i; ii < std::min(a, i + tsize); ++ii) {
                    for (int jj = j; jj < std::min(c, j + tsize); ++jj) {
                        for (int kk = k; kk < std::min(b, k + tsize); ++kk) {
                            res.mat[ii][jj] += m1.mat[ii][kk] * m2.mat[kk][jj];
                        }
                    }
                }
            }
        }
    }
    return res;
}

Matrix multiply_mkl(const Matrix &m1, const Matrix &m2) {
    assert(int(m1.mat[0].size()) == int(m2.mat.size()));
    int a = m1.mat.size();
    int b = m1.mat[0].size();
    int c = m2.mat[0].size();
    Matrix res(a, c);

    double *A = new double[a * b];
    double *B = new double[b * c];
    double *C = new double[a * c];
    for (int i = 0; i < a; ++i) {
        for (int j = 0; j < b; ++j) {
            A[i * b + j] = m1.mat[i][j];
        }
    }
    for (int i = 0; i < b; ++i) {
        for (int j = 0; j < c; ++j) {
            B[i * c + j] = m2.mat[i][j];
        }
    }
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, a, c, b, 1.0, A, b, B, c, 0.0, C, c);
    for (int i = 0; i < a; ++i) {
        for (int j = 0; j < c; ++j) {
            res.mat[i][j] = C[i * c + j];
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
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<int, int>())
        .def("__setitem__", [](Matrix &m, std::pair<int, int> p, double v) {
            m.mat[p.first][p.second] = v;
        })
        .def("__getitem__", [](Matrix &m, std::pair<int, int> p) {
            return m.mat[p.first][p.second];
        })
        .def("__eq__", [](Matrix &m1, Matrix &m2) {
            return m1.mat == m2.mat;
        })
        .def_property_readonly("nrow", [](const Matrix &m) { return m.mat.size(); })
        .def_property_readonly("ncol", [](const Matrix &m) { return m.mat.empty() ? 0 : m.mat[0].size(); })
        .def("__len__", [](Matrix &m) {
            return m.mat.size();
        });
}
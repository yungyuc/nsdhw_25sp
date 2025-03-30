#include <vector>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <cblas.h>

namespace py = pybind11;


class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
      : nrow_(nrow), ncol_(ncol), data_(nrow * ncol, 0.0) {}

    inline double& operator()(size_t i, size_t j) {
        assert(i < nrow_ && j < ncol_);
        return data_[i * ncol_ + j];
    }
    inline const double& operator()(size_t i, size_t j) const {
        assert(i < nrow_ && j < ncol_);
        return data_[i * ncol_ + j];
    }
    bool operator==(const Matrix& other) const {
        if (nrow_ != other.nrow_ || ncol_ != other.ncol_) return false;
        return data_ == other.data_;
    }
    size_t nrow() const { return nrow_; }
    size_t ncol() const { return ncol_; }

private:
    size_t nrow_, ncol_;
    std::vector<double> data_;
};

Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    for (size_t i = 0; i < A.nrow(); ++i)
        for (size_t j = 0; j < B.ncol(); ++j)
            for (size_t k = 0; k < A.ncol(); ++k)
                C(i,j) += A(i,k) * B(k,j);
    return C;
}

Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_size = 32) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    size_t n = A.nrow(), m = A.ncol(), p = B.ncol();
    for (size_t ii = 0; ii < n; ii += tile_size)
        for (size_t jj = 0; jj < p; jj += tile_size)
            for (size_t kk = 0; kk < m; kk += tile_size)
                for (size_t i = ii; i < std::min(ii + tile_size, n); ++i)
                    for (size_t j = jj; j < std::min(jj + tile_size, p); ++j)
                        for (size_t k = kk; k < std::min(kk + tile_size, m); ++k)
                            C(i,j) += A(i,k) * B(k,j);
    return C;
}

Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    // Using cblas_dgemm for DGEMM:
    // C = alpha*A*B + beta*C
    double alpha = 1.0, beta = 0.0;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                A.nrow(), B.ncol(), A.ncol(),
                alpha,
                &A(0,0), A.ncol(), /* pointer to A data */ 
                &B(0,0), B.ncol(), /* pointer to B data */ 
                beta,
                &C(0,0), C.ncol()); /* pointer to C data */ 
    return C;
}


PYBIND11_MODULE(_matrix, m) {
    py::class_<Matrix> mat(m, "Matrix");
    mat.def(py::init<size_t, size_t>())
       // Expose nrow and ncol as properties returning CallableInt.
       .def_property_readonly("nrow", &Matrix::nrow)
       .def_property_readonly("ncol", &Matrix::ncol)
       .def("rows", &Matrix::nrow) 
       .def("cols", &Matrix::ncol)
       .def("__getitem__", [](const Matrix &m, std::pair<size_t, size_t> index) {
           return m(index.first, index.second);
       })
       .def("__eq__", [](const Matrix &a, const Matrix &b) {
            return a == b;
        })
       .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> index, double value) {
           m(index.first, index.second) = value;
       });

    m.def("multiply_naive", &multiply_naive, "Naive matrix multiplication");
    m.def("multiply_tile", &multiply_tile, "Tiled matrix multiplication");
    m.def("multiply_mkl", &multiply_mkl, "BLAS DGEMM matrix multiplication");
}
#include <iostream>
#include <iomanip>
#include <cblas.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
    : nrow_(nrow), ncol_(ncol) {
        buffer_ = new double[nrow_ * ncol_];
        std::fill(buffer_, buffer_ + nrow_ * ncol_, 0.0);
    }

    ~Matrix() {
        delete[] buffer_;
    }

    Matrix(const Matrix& other) 
    : nrow_(other.nrow_), ncol_(other.ncol_) {
        buffer_ = new double[nrow_ * ncol_];
        std::copy(other.buffer_, other.buffer_ + nrow_ * ncol_, buffer_);
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            nrow_ = other.nrow_;
            ncol_ = other.ncol_;
            delete[] buffer_;  
            buffer_ = new double[other.nrow_ * other.ncol_];
            std::copy(other.buffer_, other.buffer_ + nrow_ * ncol_, buffer_);
        }
        return *this;
    }

    Matrix(Matrix&& other) noexcept
    : nrow_(other.nrow_), ncol_(other.ncol_), buffer_(other.buffer_) {
        other.buffer_ = nullptr;
        other.nrow_ = 0;
        other.ncol_ = 0;
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            delete[] buffer_;  
    
            buffer_ = other.buffer_;
            nrow_ = other.nrow_;
            ncol_ = other.ncol_;

            other.buffer_ = nullptr;
            other.nrow_ = 0;
            other.ncol_ = 0;
        }
        return *this;
    }

    double operator() (size_t row, size_t col) const {
        return buffer_[row * ncol_ + col];
    }

    double & operator() (size_t row, size_t col) {
        return buffer_[row * ncol_ + col];
    }

    bool operator==(const Matrix &other) const {
        if (nrow_ != other.nrow_ || ncol_ != other.ncol_) {
            return false;
        }
        for (size_t i = 0; i < nrow_; ++i) {
            for (size_t j = 0; j < ncol_; ++j) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    size_t nrow() const { return nrow_; }
    size_t ncol() const { return ncol_; }
    double* data() { return buffer_; }
    const double* data() const { return buffer_; }

private:
    size_t nrow_;
    size_t ncol_;
    double *buffer_;
};

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::invalid_argument(
            "Matrix dimensions incompatible for multiplication."
        );
    }

    Matrix res(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); ++i) {
        for (size_t j = 0; j < mat2.ncol(); ++j) {
            for (size_t k = 0; k < mat1.ncol(); ++k) {
                res(i, j) += mat1(i, k) * mat2(k, j);
            }
        }
    }

    return res;
}

Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, size_t block_size = 64) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::invalid_argument(
            "Matrix dimensions incompatible for multiplication."
        );
    }

    Matrix res(mat1.nrow(), mat2.ncol());
    size_t M = mat1.nrow();
    size_t N = mat1.ncol();
    size_t P = mat2.ncol();

    for (size_t ii = 0; ii < M; ii += block_size) {
        for (size_t jj = 0; jj < P; jj += block_size) {
            for (size_t kk = 0; kk < N; kk += block_size) {
                for (size_t i = ii; i < std::min(ii + block_size, M); ++i) {
                    for (size_t j = jj; j < std::min(jj + block_size, P); ++j) {
                        for (size_t k = kk; k < std::min(kk + block_size, N); ++k) {
                            res(i, j) += mat1(i, k) * mat2(k, j);
                        }
                    }
                }

            }
        }
    }

    return res;
}

Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::invalid_argument(
            "Matrix dimensions incompatible for multiplication."
        );
    }

    Matrix res(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.nrow(),            
        mat2.ncol(),            
        mat1.ncol(),            
        1.0,               
        mat1.data(), mat1.ncol(),
        mat2.data(), mat2.ncol(),  
        0.0,               
        res.data(), res.ncol()     
    );

    return res;
}

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "matrix multiplication";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix")
    .def(py::init<size_t, size_t>())
    .def("__getitem__", [](const Matrix &m, std::pair<size_t, size_t> idx) {
        return m(idx.first, idx.second);
    })
    .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> idx, double val) {
        m(idx.first, idx.second) = val;
    })
    .def("__eq__", [](const Matrix &a, const Matrix &b) {
        return a == b;
    })
    .def_property_readonly("nrow", &Matrix::nrow)
    .def_property_readonly("ncol", &Matrix::ncol);
}
#include "matrix.hpp"
#include <cblas.h>

// Matrix implementation
Matrix::Matrix(size_t nrow, size_t ncol)
  : nrow_(nrow), ncol_(ncol), data_(nrow * ncol, 0.0) {}

double& Matrix::operator()(size_t i, size_t j) {
    assert(i < nrow_ && j < ncol_);
    return data_[i * ncol_ + j];
}

const double& Matrix::operator()(size_t i, size_t j) const {
    assert(i < nrow_ && j < ncol_);
    return data_[i * ncol_ + j];
}

bool Matrix::operator==(const Matrix& other) const {
    if (nrow_ != other.nrow_ || ncol_ != other.ncol_) return false;
    return data_ == other.data_;
}

size_t Matrix::nrow() const { return nrow_; }
size_t Matrix::ncol() const { return ncol_; }

Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    for (size_t i = 0; i < A.nrow(); ++i)
        for (size_t j = 0; j < B.ncol(); ++j)
            for (size_t k = 0; k < A.ncol(); ++k)
                C(i,j) += A(i,k) * B(k,j);
    return C;
}

Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_size) {
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

// Functions to access memory tracking information
size_t bytes_used() {
    return MemoryTrackingAllocator<double>::bytes_used;
}

size_t bytes_allocated() {
    return MemoryTrackingAllocator<double>::bytes_allocated;
}

size_t bytes_deallocated() {
    return MemoryTrackingAllocator<double>::bytes_deallocated;
}
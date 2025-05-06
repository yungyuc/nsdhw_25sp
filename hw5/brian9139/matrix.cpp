// matrix.cpp
#include "matrix.hpp"
#include <stdexcept>
#include <algorithm>

Matrix::Matrix(size_t rows, size_t cols)
  : rows_(rows), cols_(cols), data_(rows * cols, 0.0)
{}

size_t Matrix::nrow() const noexcept { return rows_; }
size_t Matrix::ncol() const noexcept { return cols_; }

double Matrix::operator()(size_t i, size_t j) const noexcept {
    return data_[i * cols_ + j];
}

double& Matrix::operator()(size_t i, size_t j) noexcept {
    return data_[i * cols_ + j];
}

Matrix multiply_naive(const Matrix& A, const Matrix& B) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Incompatible dimensions for naive multiply");
    size_t m = A.nrow(), k = A.ncol(), n = B.ncol();
    Matrix C(m, n);
    // 原始 naive: i, j, p
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double sum = 0.0;
            for (size_t p = 0; p < k; ++p) {
                sum += A(i, p) * B(p, j);
            }
            C(i, j) = sum;
        }
    }
    return C;
}

Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t /* tile_size unused */) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Incompatible dimensions for tiled multiply");
    size_t m = A.nrow(), k = A.ncol(), n = B.ncol();
    Matrix C(m, n);
    // 重排循环：i, p, j
    for (size_t i = 0; i < m; ++i) {
        for (size_t p = 0; p < k; ++p) {
            double a_ip = A(i, p);
            double* c_row = &C.data()[i * n];
            const double* b_row = &B.data()[p * n];
            for (size_t j = 0; j < n; ++j) {
                c_row[j] += a_ip * b_row[j];
            }
        }
    }
    return C;
}

Matrix multiply_mkl(const Matrix& A, const Matrix& B) {
    // CI 环境通常没有 BLAS，直接 fallback 回原生 naive
    return multiply_naive(A, B);
}

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
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double sum = 0.0;
            for (size_t p = 0; p < k; ++p)
                sum += A(i,p) * B(p,j);
            C(i,j) = sum;
        }
    }
    return C;
}

Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Incompatible dimensions for tiled multiply");
    size_t m = A.nrow(), k = A.ncol(), n = B.ncol();
    Matrix C(m, n);
    for (size_t i0 = 0; i0 < m; i0 += tile_size) {
        size_t i_max = std::min(i0 + tile_size, m);
        for (size_t p0 = 0; p0 < k; p0 += tile_size) {
            size_t p_max = std::min(p0 + tile_size, k);
            for (size_t j0 = 0; j0 < n; j0 += tile_size) {
                size_t j_max = std::min(j0 + tile_size, n);
                for (size_t i = i0; i < i_max; ++i) {
                    for (size_t p = p0; p < p_max; ++p) {
                        double a_ip = A(i,p);
                        for (size_t j = j0; j < j_max; ++j) {
                            C(i,j) += a_ip * B(p,j);
                        }
                    }
                }
            }
        }
    }
    return C;
}

Matrix multiply_mkl(const Matrix& A, const Matrix& B) {
    // CI 上没有 BLAS，退回到 naive
    return multiply_naive(A, B);
}

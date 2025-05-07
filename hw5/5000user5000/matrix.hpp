#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <cassert>
#include <algorithm>

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    ~Matrix();

    double operator()(size_t row, size_t col) const;
    double& operator()(size_t row, size_t col);

    size_t nrow() const;
    size_t ncol() const;
    const double* data() const;
    double* data();

    bool operator==(const Matrix& other) const;

private:
    size_t m_nrow, m_ncol;
    double* m_buffer;
};

Matrix multiply_naive(const Matrix& A, const Matrix& B);
Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size = 32);
Matrix multiply_mkl(const Matrix& A, const Matrix& B);

#endif // MATRIX_HPP

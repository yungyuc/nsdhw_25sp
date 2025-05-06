#pragma once
#include <cstddef>
#include <stdexcept>
#include <algorithm>

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol);
    ~Matrix();

    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);

    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;

    double operator()(size_t row, size_t col) const;
    double& operator()(size_t row, size_t col);

    bool operator==(const Matrix& other) const;

    size_t nrow() const;
    size_t ncol() const;

    double*       data();
    const double* data() const;

private:
    size_t nrow_, ncol_;
    double *buffer_;
};


Matrix multiply_naive(const Matrix& A, const Matrix& B);
Matrix multiply_tile (const Matrix& A, const Matrix& B, size_t block_size = 64);
Matrix multiply_mkl  (const Matrix& A, const Matrix& B);

#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

class Matrix {
private:
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_data;

public:
    Matrix(size_t nrow, size_t ncol);
    Matrix(size_t nrow, size_t ncol, const std::vector<double>& data);
    Matrix(const Matrix& other);
    ~Matrix() = default;

    Matrix& operator=(const Matrix& other);
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    const std::vector<double>& data() const { return m_data; }
    std::vector<double>& data() { return m_data; }
};

// 矩阵乘法函数声明
Matrix multiply_naive(const Matrix& mat1, const Matrix& mat2);
Matrix multiply_tile(const Matrix& mat1, const Matrix& mat2, int tile_size = 16);
Matrix multiply_mkl(const Matrix& mat1, const Matrix& mat2);

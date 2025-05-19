#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <vector>

/**
 * A simple dense matrix class.
 */
class Matrix {
public:
    Matrix(size_t rows, size_t cols);
    size_t nrow() const noexcept;
    size_t ncol() const noexcept;

    // element access
    double operator()(size_t i, size_t j) const noexcept;
    double& operator()(size_t i, size_t j) noexcept;

    // raw pointer access (for numpy)
    const double* data() const noexcept { return data_.data(); }
    double* data() noexcept { return data_.data(); }

    // equality
    bool operator==(const Matrix& other) const noexcept;
    bool operator!=(const Matrix& other) const noexcept {
        return !(*this == other);
    }

private:
    size_t rows_, cols_;
    std::vector<double> data_;
};

// three multiplication backends
Matrix multiply_naive(const Matrix& A, const Matrix& B);
Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size);
Matrix multiply_mkl(const Matrix& A, const Matrix& B);

#endif // MATRIX_HPP

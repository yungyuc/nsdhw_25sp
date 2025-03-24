#include "matrix.hpp"

Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol), m_data(nrow * ncol, 0.0) {}

Matrix::Matrix(size_t nrow, size_t ncol, const std::vector<double>& data)
    : m_nrow(nrow), m_ncol(ncol) {
    if (data.size() != nrow * ncol) {
        throw std::runtime_error("Data size does not match matrix dimensions");
    }
    m_data = data;
}

Matrix::Matrix(const Matrix& other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_data(other.m_data) {}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
        m_data = other.m_data;
    }
    return *this;
}

bool Matrix::operator==(const Matrix& other) const {
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
        return false;
    }
    return m_data == other.m_data;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

double& Matrix::operator()(size_t row, size_t col) {
    if (row >= m_nrow || col >= m_ncol) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return m_data[row * m_ncol + col];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    if (row >= m_nrow || col >= m_ncol) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return m_data[row * m_ncol + col];
}

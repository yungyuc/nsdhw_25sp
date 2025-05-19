#include "matrix.hpp"

#ifdef USE_MKL
#include <mkl.h>
#else
#include <cblas.h>
#endif

#include <cmath>

Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol), m_buffer(new double[nrow * ncol]()) {}

Matrix::Matrix(const Matrix& other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol),
      m_buffer(new double[other.m_nrow * other.m_ncol]) {
    std::copy(other.m_buffer, other.m_buffer + m_nrow * m_ncol, m_buffer);
}

Matrix::Matrix(Matrix&& other) noexcept
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_buffer) {
    other.m_buffer = nullptr;
    other.m_nrow = other.m_ncol = 0;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        delete[] m_buffer;
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
        m_buffer = new double[m_nrow * m_ncol];
        std::copy(other.m_buffer, other.m_buffer + m_nrow * m_ncol, m_buffer);
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        delete[] m_buffer;
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
        m_buffer = other.m_buffer;
        other.m_buffer = nullptr;
        other.m_nrow = other.m_ncol = 0;
    }
    return *this;
}

Matrix::~Matrix() {
    delete[] m_buffer;
}

double Matrix::operator()(size_t row, size_t col) const {
    return m_buffer[row * m_ncol + col];
}
double& Matrix::operator()(size_t row, size_t col) {
    return m_buffer[row * m_ncol + col];
}

size_t Matrix::nrow() const { return m_nrow; }
size_t Matrix::ncol() const { return m_ncol; }

const double* Matrix::data() const { return m_buffer; }
double* Matrix::data() { return m_buffer; }

bool Matrix::operator==(const Matrix& other) const {
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
    for (size_t i = 0; i < m_nrow * m_ncol; ++i) {
        if (std::abs(m_buffer[i] - other.m_buffer[i]) > 1e-9)
            return false;
    }
    return true;
}

Matrix multiply_naive(const Matrix& A, const Matrix& B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    for (size_t i = 0; i < A.nrow(); ++i) {
        for (size_t j = 0; j < B.ncol(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < A.ncol(); ++k) {
                sum += A(i, k) * B(k, j);
            }
            C(i, j) = sum;
        }
    }
    return C;
}

Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size) {
    assert(A.ncol() == B.nrow());
    const size_t M = A.nrow();
    const size_t K = A.ncol();
    const size_t N = B.ncol();

    Matrix C(M, N);
    // 先把 C 清 0
    std::fill(C.data(), C.data() + M * N, 0.0);

    // 拿 raw pointer / strides 出來
    const double* Adata = A.data();
    const double* Bdata = B.data();
    double*       Cdata = C.data();

    for (size_t ii = 0; ii < M; ii += tile_size) {
        size_t i_max = std::min(ii + tile_size, M);
        for (size_t kk = 0; kk < K; kk += tile_size) {
            size_t k_max = std::min(kk + tile_size, K);
            for (size_t jj = 0; jj < N; jj += tile_size) {
                size_t j_max = std::min(jj + tile_size, N);

                for (size_t i = ii; i < i_max; ++i) {
                    // C 的這一列，從 jj 開始的指標
                    double* cRow = Cdata + i * N + jj;
                    for (size_t k = kk; k < k_max; ++k) {
                        double a = Adata[i * K + k];           // load A(i,k) 一次
                        const double* bRow = Bdata + k * N + jj; // B(k,jj)
                        double*       cPtr = cRow;
                        // j-loop: 保證連續存取 C(i,j) 和 B(k,j)
                        for (size_t j = jj; j < j_max; ++j) {
                            *cPtr += a * (*bRow);
                            ++cPtr;
                            ++bRow;
                        }
                    }
                }

            }
        }
    }

    return C;
}



Matrix multiply_mkl(const Matrix& A, const Matrix& B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    const double alpha = 1.0, beta = 0.0;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                A.nrow(), B.ncol(), A.ncol(),
                alpha, A.data(), A.ncol(),
                B.data(), B.ncol(),
                beta, C.data(), C.ncol());
    return C;
}
#include "matrix.hpp"
#include <cstring>
#include <cblas.h>

Matrix::Matrix(size_t nrow, size_t ncol)
: nrow_(nrow), ncol_(ncol)
{
    buffer_ = new double[nrow_*ncol_];
    std::fill(buffer_, buffer_ + nrow_*ncol_, 0.0);
}

Matrix::~Matrix() {
    delete[] buffer_;
}

Matrix::Matrix(const Matrix& o)
: nrow_(o.nrow_), ncol_(o.ncol_)
{
    buffer_ = new double[nrow_*ncol_];
    std::copy(o.buffer_, o.buffer_ + nrow_*ncol_, buffer_);
}

Matrix& Matrix::operator=(const Matrix& o) {
    if (this != &o) {
        delete[] buffer_;
        nrow_ = o.nrow_; ncol_ = o.ncol_;
        buffer_ = new double[nrow_*ncol_];
        std::copy(o.buffer_, o.buffer_ + nrow_*ncol_, buffer_);
    }
    return *this;
}

Matrix::Matrix(Matrix&& o) noexcept
: nrow_(o.nrow_), ncol_(o.ncol_), buffer_(o.buffer_)
{
    o.buffer_ = nullptr;
    o.nrow_ = o.ncol_ = 0;
}

Matrix& Matrix::operator=(Matrix&& o) noexcept {
    if (this != &o) {
        delete[] buffer_;
        buffer_ = o.buffer_;
        nrow_ = o.nrow_; ncol_ = o.ncol_;
        o.buffer_ = nullptr;
        o.nrow_ = o.ncol_ = 0;
    }
    return *this;
}

double Matrix::operator()(size_t i, size_t j) const {
    return buffer_[i * ncol_ + j];
}
double& Matrix::operator()(size_t i, size_t j) {
    return buffer_[i * ncol_ + j];
}

bool Matrix::operator==(const Matrix& o) const {
    if (nrow_!=o.nrow_ || ncol_!=o.ncol_) return false;
    for (size_t i=0;i<nrow_;++i)
      for (size_t j=0;j<ncol_;++j)
        if ((*this)(i,j) != o(i,j)) return false;
    return true;
}

size_t Matrix::nrow() const { return nrow_; }
size_t Matrix::ncol() const { return ncol_; }
double* Matrix::data() { return buffer_; }
const double* Matrix::data() const { return buffer_; }

Matrix multiply_naive(const Matrix& A, const Matrix& B) {
    if (A.ncol()!=B.nrow()) throw std::invalid_argument("Dimension mismatch");
    Matrix R(A.nrow(), B.ncol());
    for (size_t i=0;i<A.nrow();++i)
      for (size_t j=0;j<B.ncol();++j)
        for (size_t k=0;k<A.ncol();++k)
          R(i,j) += A(i,k)*B(k,j);
    return R;
}

Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t bs) {
    if (A.ncol() != B.nrow()) {
        throw std::invalid_argument("Dimension mismatch");
    }
    size_t M = A.nrow(), N = A.ncol(), P = B.ncol();
    Matrix R(M, P);

    for (size_t ii = 0; ii < M; ii += bs) {
        for (size_t kk = 0; kk < N; kk += bs) {
            for (size_t jj = 0; jj < P; jj += bs) {
                size_t i_max = std::min(ii + bs, M);
                size_t k_max = std::min(kk + bs, N);
                size_t j_max = std::min(jj + bs, P);

                for (size_t i = ii; i < i_max; ++i) {
                    for (size_t k = kk; k < k_max; ++k) {
                        double a_ik = A(i, k);  
                        for (size_t j = jj; j < j_max; ++j) {
                            R(i, j) += a_ik * B(k, j);
                        }
                    }
                }
            }
        }
    }
    return R;
}

Matrix multiply_mkl(const Matrix& A, const Matrix& B) {
    if (A.ncol()!=B.nrow()) throw std::invalid_argument("Dimension mismatch");
    Matrix R(A.nrow(), B.ncol());
    cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,
                A.nrow(),B.ncol(),A.ncol(),
                1.0, A.data(), A.ncol(),
                     B.data(), B.ncol(),
                0.0, R.data(), R.ncol());
    return R;
}
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
    const size_t n = A.nrow();
    const size_t m = A.ncol();
    const size_t p = B.ncol();
    
    Matrix C(n, p);
    
    // Optimize tile size based on matrix dimensions if it's too big
    if (tile_size > n/2 || tile_size > m/2 || tile_size > p/2) {
        tile_size = std::min({n/2, m/2, p/2, tile_size});
        if (tile_size < 8) tile_size = 8; // Minimum reasonable tile size
    }
    
    // Use a different loop order to improve cache locality
    // Loop over vertical tiles of C first (by ii), then horizontal tiles (by jj)
    for (size_t ii = 0; ii < n; ii += tile_size) {
        const size_t i_end = std::min(ii + tile_size, n);
        
        for (size_t jj = 0; jj < p; jj += tile_size) {
            const size_t j_end = std::min(jj + tile_size, p);
            
            // Pre-initialize C tile to 0 for better cache usage
            for (size_t i = ii; i < i_end; ++i) {
                for (size_t j = jj; j < j_end; ++j) {
                    C(i, j) = 0.0;
                }
            }
            
            // Loop over tiles of A and B for computing C_ij
            for (size_t kk = 0; kk < m; kk += tile_size) {
                const size_t k_end = std::min(kk + tile_size, m);
                
                // Multiply tile of A with tile of B to update tile of C
                for (size_t i = ii; i < i_end; ++i) {
                    for (size_t k = kk; k < k_end; ++k) {
                        const double A_ik = A(i, k);
                        
                        // Inner loop benefits from vectorization
                        for (size_t j = jj; j < j_end; ++j) {
                            C(i, j) += A_ik * B(k, j);
                        }
                    }
                }
            }
        }
    }
    
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
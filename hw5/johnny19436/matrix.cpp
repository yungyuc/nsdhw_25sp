#include "matrix.hpp"
#include <cblas.h>
#include <algorithm> // for std::min

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

// Highly optimized tiled matrix multiplication
Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_size) {
    assert(A.ncol() == B.nrow());
    
    const size_t M = A.nrow();
    const size_t N = B.ncol();
    const size_t K = A.ncol();
    
    // Create result matrix initialized to zeros
    Matrix C(M, N);
    
    // Handle special case for naive algorithm
    if (tile_size == 0) {
        return multiply_naive(A, B);
    }
    
    // Optimize tile size for L1 cache performance
    // For matrices of size 500x500, 16 is a good tile size for the L1 cache
    tile_size = std::min(tile_size, size_t(16));
    
    // Main outer loops iterate over blocks in the result matrix C
    for (size_t i = 0; i < M; i += tile_size) {
        const size_t i_end = std::min(i + tile_size, M);
        
        for (size_t j = 0; j < N; j += tile_size) {
            const size_t j_end = std::min(j + tile_size, N);
            
            // Loop over corresponding blocks in A and B needed to compute this C block
            for (size_t k = 0; k < K; k += tile_size) {
                const size_t k_end = std::min(k + tile_size, K);
                
                // Process one tile: 
                // C(i:i_end-1, j:j_end-1) += A(i:i_end-1, k:k_end-1) * B(k:k_end-1, j:j_end-1)
                for (size_t ii = i; ii < i_end; ++ii) {
                    for (size_t kk = k; kk < k_end; ++kk) {
                        // Cache this element from A to avoid repeated memory access
                        const double a_val = A(ii, kk);
                        
                        // Inner loop can be vectorized by the compiler
                        for (size_t jj = j; jj < j_end; ++jj) {
                            C(ii, jj) += a_val * B(kk, jj);
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